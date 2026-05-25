# ---
# jupyter:
#   jupytext:
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#       jupytext_version: 1.19.2
#   kernelspec:
#     display_name: ml_ufpr
#     language: python
#     name: python3
# ---

# %%
import torch
import torch.nn as nn
from torchvision import datasets, transforms, models
from torch.utils.data import DataLoader
import os
import csv
from tqdm import tqdm
from sklearn.decomposition import PCA
from sklearn.preprocessing import MinMaxScaler
import pennylane as qp
from pennylane import numpy as pnp
import numpy as np
import torch.optim as optim

# ==== Config ====

DEVICE = (
    "mps" if torch.backends.mps.is_available()
    else "cuda" if torch.cuda.is_available()
    else "cpu"
)

DATA_DIR = "../../datasets"
BATCH_SIZE = 128
SEED       = 42

torch.manual_seed(SEED)
np.random.seed(SEED)

print("Configurações setadas.")

# %%
# ==== Carregamento do Modelo - MobileNetV3 Small ====

print("Carregando modelo...")

weights = models.MobileNet_V3_Small_Weights.DEFAULT
base_transform = weights.transforms()

model = models.mobilenet_v3_small(weights=weights)

# Substitui a camada final por uma identidade
model.classifier = nn.Identity()

model = model.to(DEVICE)
model.eval()

print("Modelo carregado.")

# %%
# ===== Carrega dataset ====

# O MNIST é em tons de cinza. Converter a imagem para RGB (3 canais) antes de 
# aplicar o redimensionamento e normalização
transform = transforms.Compose([
    transforms.Lambda(lambda img: img.convert("RGB")),
    base_transform
])

print("Baixando e carregando dataset MNIST...")
train_dataset = datasets.MNIST(root=DATA_DIR, train=True, download=True, transform=transform)
test_dataset = datasets.MNIST(root=DATA_DIR, train=False, download=True, transform=transform)

train_loader = DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=False)
test_loader = DataLoader(test_dataset, batch_size=BATCH_SIZE, shuffle=False)

print("Download completo.")


# %%
# ==== Extração de características ====
def extract_features(loader, desc_text):
    features, labels = [], []

    with torch.no_grad():
        for images, lbls in tqdm(loader, desc=desc_text, unit="batch"):
            images = images.to(DEVICE)

            # passa pela rede
            feats = model(images)

            # garante que o formato é 1d por imagem
            if feats.dim() > 2:
                feats = feats.mean(dim=1)
            
            features.append(feats.cpu().numpy())
            labels.append(lbls.cpu().numpy())

    return np.vstack(features), np.hstack(labels)

# ==== Salvar de características vetor em CSV ====
def save_csv(features, labels, filename):
    num_features = features.shape[1]
    header = ["label"] + [f"f{i}" for i in range(num_features)]

    with open(filename, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(header)
        for x, y in zip(features, labels):
            writer.writerow([y] + x.tolist())

# ==== Carrega vetor de características de um CSV ====



# %%
# === Extrai o vetor de características ====

X_train, y_train = extract_features(train_loader, "Extraindo treino")
X_test, y_test = extract_features(test_loader, "Extraindo teste")

print("\nFormato dos vetores extraídos:")
print("Treino:", X_train.shape)
print("Teste:", X_test.shape)

# %%
# ==== Aplica PCA no feature vector ====

pca = PCA(n_components=10)

X_train = pca.fit_transform(X_train)
X_test = pca.transform(X_test)

print("Formato após PCA:")
print("Train shape:", X_train.shape)
print("Test shape:", X_test.shape)

# Pega variancia total após redução de dimensionalidade
variance = sum(pca.explained_variance_ratio_)
print(f"Variância total mantida: {variance * 100:.2f}%")

# %%
# ==== Preparação para Treino Quântico ====

# Normalizar dados para qp 
# Os dados do vetor serão transformados em ângulos para os qubits, então eles 
# devem fica em um intervalo de 0 a pi radianos.
scaler = MinMaxScaler(feature_range=(0, np.pi))

# convertendo para tensores do pennylane
X_train_norm = pnp.array(scaler.fit_transform(X_train), requires_grad=False) 
X_test_norm = pnp.array(scaler.transform(X_test), requires_grad=False)
y_train = pnp.array(y_train, requires_grad=False)
y_test = pnp.array(y_test, requires_grad=False) 

n_qubits = X_train_norm.shape[1]

# Config do dispositivo quântico
dev = qp.device("default.qubit", wires=n_qubits)

def state_preparation(x):
    qp.AngleEmbedding(features=x, wires=range(n_qubits), rotation='X')

@qp.qnode(dev)
def quantum_neural_network(weights, x):
    # 1. Carrega os dados da imagem
    state_preparation(x)

    # 2. Passa pelas camadas treináveis
    # Os pesos são os ângulos de rotação
    qp.StronglyEntanglingLayers(weights=weights, wires=range(n_qubits))

    # 3. Medição
    # Medidos o valor esperado no eixo Z para todos 10 qubits
    # Isso datá um vetor de 10 valores contínuos entre -1 e 1
    return [qp.expval(qp.PauliZ(i)) for i in range(n_qubits)]

num_layers = 3
shape_weights = (num_layers, n_qubits, 3)

# inicializa pesos aleatoriamente
starting_weights = 0.01 * pnp.random.randn(*shape_weights, requires_grad=True)


# ==== Funções da Etapa de Treinamento ====

# Função de ativação (Softmax)
def softmax(x):
    exp_x = pnp.exp(x - pnp.max(x))
    return exp_x / pnp.sum(exp_x)

# Previsão para uma única imagem
def predict(weights, x):
    # roda o circuito quântico
    q_out = quantum_neural_network(weights, x)

    # empilha a lista de respostas em um tensor
    q_out = pnp.stack(q_out)

    #transforma os valores em probabilidades
    return softmax(q_out)

# Função de custo (Cross-Entropy)
def cost_function(weights, X_batch, Y_batch):
    loss = 0.0
    for i in range(len(X_batch)):
        p = predict(weights, X_batch[i])
        correct_label = int(Y_batch[i])

        # adicionar 1e-7 para evitar erro de log(0)
        loss -= pnp.log(p[correct_label] + 1e-7)
    
    return loss / len(X_batch)

# Função auxiliar de acurácia
def accuracy(weights, X_batch, Y_batch):
    correct_guesses = 0
    for i in range(len(X_batch)):
        p = predict(weights, X_batch[i])
        forecast = pnp.argmax(p)

        if forecast == int(Y_batch[i]):
            correct_guesses += 1
    
    return correct_guesses / len(X_batch)


# %%
# ==== Otimizador e Execução ====

opt = qp.AdamOptimizer(stepsize=0.1)

# Usar um subconjunto para treinar pois é muito grande
SAMPLES = 60000

X_train_subset = X_train_norm[:SAMPLES]
y_train_subset = y_train[:SAMPLES]

EPOCHS = 50
BATCH_SIZE = 20

print("Iniciando treinamento híbrido...\n")

for it in range(EPOCHS):
    # embaralha os dados no inicio de cada época
    idx = np.random.permutation(len(X_train_subset))
    X_train_subset = X_train_subset[idx]
    y_train_subset = y_train_subset[idx]

    # processamento em batches
    for i in range(0, len(X_train_subset), BATCH_SIZE):
        X_batch = X_train_subset[i : i + BATCH_SIZE]
        y_batch = y_train_subset[i : i + BATCH_SIZE]

        # calcula o gradiente da matrix e atualiza os pesos
        starting_weights = opt.step(lambda w: cost_function(w, X_batch, y_batch), starting_weights)

    acc = accuracy(starting_weights, X_train_subset, y_train_subset)
    cost = cost_function(starting_weights, X_train_subset, y_train_subset)
    print(f"Época {it+1:2d} | Custo: {cost:.4f} | Acurácia (Treino): {acc:.4f}")

# %%
# ==== Treinamento clássico para comparação - MobileNetV3 Small ====

# Recria o modelo do MobileNetV3
classic_model = nn.Sequential(
    nn.Linear(n_qubits, 1024),
    nn.Hardswish(),
    nn.Dropout(p=0.2, inplace=True),
    nn.Linear(1024, 10) # 10 saidas para os digitos
).to(DEVICE)

criterion = nn.CrossEntropyLoss()
classic_optimizer = optim.Adam(classic_model.parameters(), lr=0.01)

# Converte os tensores pennylane de volta para pytorch
X_torch = torch.tensor(X_train_subset.numpy(), dtype=torch.float32).to(DEVICE)
y_torch = torch.tensor(y_train_subset.numpy(), dtype=torch.long).to(DEVICE)

classic_model.train()

print("Iniciando treinamento clássico...\n")

# Loop de treino
for epoch in range(EPOCHS):
    idxs = torch.randperm(X_torch.size(0))
    X_shuffled = X_torch[idxs]
    y_shuffled = y_torch[idxs]

    loss_total = 0.0
    correct_guesses = 0

    # processamento em batches
    for i in range(0, len(X_shuffled), BATCH_SIZE):
        X_b = X_shuffled[i : i + BATCH_SIZE]
        y_b = y_shuffled[i : i + BATCH_SIZE]

        classic_optimizer.zero_grad()

        # passa pela rede
        outputs = classic_model(X_b)
        loss = criterion(outputs, y_b)

        # backprop
        loss.backward()
        classic_optimizer.step()

        # metricas
        loss_total += loss.item() * X_b.size(0)
        guesses = torch.argmax(outputs, dim=1)
        correct_guesses += (guesses == y_b).sum().item()

    acc_epoch = correct_guesses / len(X_shuffled)
    cost_epoch = loss_total / len(X_shuffled)

    print(f"Época Clássica {epoch+1:2d} | Custo: {cost_epoch:.4f} | Acurácia (Treino): {acc_epoch:.4f}")

# %% [markdown]
# # Resultados
#
# **10 épocas com 200 amostras**
#
# Quântico:  
# Época  1 | Custo: 2.2609 | Acurácia (Treino): 0.2050  
# Época  2 | Custo: 2.2338 | Acurácia (Treino): 0.2550  
# Época  3 | Custo: 2.2124 | Acurácia (Treino): 0.3800  
# Época  4 | Custo: 2.1939 | Acurácia (Treino): 0.3150  
# Época  5 | Custo: 2.1753 | Acurácia (Treino): 0.3850  
# Época  6 | Custo: 2.1654 | Acurácia (Treino): 0.3950  
# Época  7 | Custo: 2.1565 | Acurácia (Treino): 0.4100  
# Época  8 | Custo: 2.1540 | Acurácia (Treino): 0.3600  
# Época  9 | Custo: 2.1530 | Acurácia (Treino): 0.3900  
# Época 10 | Custo: 2.1583 | Acurácia (Treino): 0.4000  
#
# Clássico:  
# Época Clássica  1 | Custo: 2.8522 | Acurácia (Treino): 0.1100   
# Época Clássica  2 | Custo: 2.7176 | Acurácia (Treino): 0.2100  
# Época Clássica  3 | Custo: 2.1554 | Acurácia (Treino): 0.3750  
# Época Clássica  4 | Custo: 1.2791 | Acurácia (Treino): 0.6000  
# Época Clássica  5 | Custo: 1.0557 | Acurácia (Treino): 0.6950  
# Época Clássica  6 | Custo: 0.9656 | Acurácia (Treino): 0.6700  
# Época Clássica  7 | Custo: 0.7404 | Acurácia (Treino): 0.8050  
# Época Clássica  8 | Custo: 0.6854 | Acurácia (Treino): 0.8050  
# Época Clássica  9 | Custo: 0.6292 | Acurácia (Treino): 0.7850  
# Época Clássica 10 | Custo: 0.5758 | Acurácia (Treino): 0.8000  
