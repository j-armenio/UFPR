from sklearn.neighbors import KNeighborsClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score
from sklearn.datasets import load_svmlight_file
import numpy as np
import time
import os

# --------------------------------------------------
# Carrega as bases
# --------------------------------------------------
path = "../datasets/lab2"
train_path = os.path.join(path, "train.txt")
test_path = os.path.join(path, "test.txt")

# Recebe um caminho para uma base e retorna as features (X) e rotulo (y)
def load_data(file_path):
    # separa X(features) e y(labels)
    X, y = load_svmlight_file(file_path)

    # REVISAR ISSO: alguns modelos (como NB e LDA) não aceitam matrizes esparsas. Como o
    # dataset tem apenas 132 features, converter para um array denso é seguro
    # e evita erros.
    return X.toarray(), y

X_train_full, y_train_full = load_data(train_path)
X_test, y_test = load_data(test_path)

print(f"Base de Treino: {X_train_full.shape[0]} exemplos com {X_train_full.shape[1]} atributos.")
print(f"Base de Teste: {X_test.shape[0]} exemplos.")

# --------------------------------------------------
# Carrega o classificador escolhido 
# --------------------------------------------------
# Retorna o classificador selecionado
def get_classifier(name):
    if name == 'knn':
        return KNeighborsClassifier(n_neighbors=5)
    elif name == 'nb':
        return GaussianNB()
    elif name == 'lda':
        return LinearDiscriminantAnalysis()
    elif name == 'lr':
        return LogisticRegression(max_iter=1000)
    elif name == 'dt':
        return DecisionTreeClassifier()
    elif name == 'svm':
        # SVM pode ser lento com 20k exemplos, 
        # dependendo do kernel escolhido (o padrão é 'rbf')
        return SVC()
    else:
        raise ValueError("Classificador não reconhecido")

# Opções: knn, nb, lda, lr, dt, svm
model_choice = 'svm'

print(f"Modelo selecionado: {model_choice}")

# --------------------------------------------------
# Loop de treinamento incremental (+1000)
# --------------------------------------------------
# Configs do loop
step = 1000
max_samples = X_train_full.shape[0] # 20000
train_sizes = range(step, max_samples + 1, step)

# listas para armazenar resultados
results_accuracy = []
results_time = []

print(f"Iniciando treinamento incremental...")
print("-" * 30)

for size in train_sizes:
    # 1. fatiar a base de treinamento
    X_train_sub = X_train_full[:size]
    y_train_sub = y_train_full[:size]

    # 2. treina o modelo
    clf = get_classifier(model_choice)
    clf.fit(X_train_sub, y_train_sub)

    # 3. testar e medir o tempo de inferência (predição
    start_time = time.time()
    y_pred = clf.predict(X_test)
    end_time = time.time()

    # 4. calcular as métricas
    acc = accuracy_score(y_test, y_pred)
    duration = end_time - start_time

    results_accuracy.append(acc)
    results_time.append(duration)

    print(f"Treino com {size} exemplos | Acurácia: {acc:.4f} | Tempo para teste: {duration:.4f}s")

print("-" * 30)
print("Treinamento concluído.")