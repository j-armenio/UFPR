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
import matplotlib.pyplot as plt

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

# ======================================================
# Funções auxiliares 
# ======================================================
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

# ======================================================
# Experimento 1: Comparação em blocos de 1000 exemplos para cada modelo
# ======================================================
def block_model_comparator(models_list, train_sizes):    
    # dicionarios para guardar os resultados de todos modelos
    all_accuracies = {}
    all_times = {}

    print(f"\nIniciando experimento: Comparação de modelos em blocos...")
    print(f"Tamanhos para treino: {train_sizes}")
    print("=" * 50)

    # Loop de escolha do modelo
    for model_name in models_list:
        accuracies = []
        times = []

        print(f"\n> Avaliando: {model_name.upper()}")

        # Loop de incremento de dados
        for size in train_sizes:
            # 1. fatiar a base de treinamento
            X_train_sub = X_train_full[:size]
            y_train_sub = y_train_full[:size]

            # 2. treina o modelo
            clf = get_classifier(model_name)
            clf.fit(X_train_sub, y_train_sub)

            # 3. testar e medir o tempo de inferência (predição)
            start_time = time.time()
            y_pred = clf.predict(X_test)
            end_time = time.time()

            # 4. calcular as métricas
            acc = accuracy_score(y_test, y_pred)
            duration = end_time - start_time

            accuracies.append(acc)
            times.append(duration)

            print(f"    [{model_name}] Tamanho {size}: Acc {acc:.4f}: Tempo: {duration:.2f}")

        all_accuracies[model_name] = accuracies
        all_times[model_name] = times

    print("\n" + "=" * 50)
    print("Treinamento concluído, plotando gráfico...")

    # --------------------
    # Plotando o gráfico
    # --------------------
    plt.figure(figsize=(12, 7))

    for model_name in models_list:
        plt.plot(train_sizes, all_accuracies[model_name], label=model_name.upper(), marker='o', markersize=4)

    plt.xlabel('Número de Exemplos de Treino')
    plt.ylabel('Acurácia na Base de Teste')
    plt.xticks(train_sizes, rotation=45)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    plt.tight_layout()

    # Salva e exibe
    # plt.savefig('comparativo_classificadores.png')
    plt.show()
    
    print("\nExperimento concluído.")

# Configs do experimento
models_to_run = ['knn', 'nb', 'lda', 'lr', 'dt', 'svm']

step = 1000
max_samples = X_train_full.shape[0] # 20000
train_sizes = range(step, max_samples + 1, step)

# block_model_comparator(models_to_run, train_sizes)

# ======================================================
# Experimento 2: Melhor desempenho com poucos dados
# ======================================================
train_sizes = [20, 100, 200, 500, 800, 1000]
# block_model_comparator(models_to_run, train_sizes)

# ======================================================
# Experimento 3: Classificador mais rápido para classificar os 58k exemplos de teste
# ======================================================
def time_comparator(models_list, train_size):
    print(f"\nIniciando experimento: Comparação de tempo de classificação...")
    print(f"Tamanho para treino: {train_size}")
    print("=" * 50)

    accuracies = {}
    times = {}

    # Loop de escolha do modelo
    for model_name in models_list:
        print(f"\n> Avaliando: {model_name.upper()}")

        # 1. fatia a base de treinamento
        X_train_sub = X_train_full[:train_size]
        y_train_sub = y_train_full[:train_size]

        # 2. treina o modelo
        clf = get_classifier(model_name)
        clf.fit(X_train_sub, y_train_sub)

        # 3. testar e medir o tempo de inferência (predição)
        start_time = time.time()
        y_pred = clf.predict(X_test)
        end_time = time.time()

        # 4. calcular as métricas
        acc = accuracy_score(y_test, y_pred)
        duration = end_time - start_time

        # 5. guarda os resultados
        accuracies[model_name] = acc
        times[model_name] = duration

        print(f"    [{model_name}] Acc {acc:.4f}: Tempo: {duration:.2f}")

    print("\n" + "=" * 50)
    print("Treinamento concluído, plotando gráfico...")

    # --------------------
    # Plotando o gráfico
    # --------------------
    # Extraindo nomes e valores para o plot
    names = [n.upper() for n in times.keys()]
    values = list(times.values())

    plt.figure(figsize=(10, 6))
    
    # Criando o gráfico de barras
    bars = plt.bar(names, values, color='salmon', edgecolor='darkred')

    # Adicionando os valores exatos acima de cada barra
    for bar in bars:
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2, yval, f'{yval:.3f}s', 
                 va='bottom', ha='center', fontsize=10, fontweight='bold')
    
    plt.ylabel('Tempo em Segundos', fontsize=12)
    plt.xlabel('Classificadores', fontsize=12)
    plt.grid(axis='y', linestyle='--', alpha=0.6)
    
    # Se a diferença for muito brutal (ex: KNN levar 20s e NB levar 0.01s),
    # descomente a linha abaixo para usar escala logarítmica:
    plt.yscale('log')

    plt.tight_layout()
    plt.show()
    
    print("\nExperimento concluído.")

# time_comparator(models_to_run, max_samples)

# ======================================================
# Experimento 4: Impacto dos hiperparâmetros
# ======================================================
