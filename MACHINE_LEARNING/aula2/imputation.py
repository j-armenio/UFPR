import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.impute import SimpleImputer, KNNImputer
from sklearn.experimental import enable_iterative_imputer
from sklearn.impute import IterativeImputer
from sklearn.ensemble import RandomForestRegressor
from sklearn.linear_model import LinearRegression

n_samples = 1000

# Gera um dataset de 4 features, em que cada uma segue uma distribuição  
np.random.seed(42)
X = pd.DataFrame({
    "Feature1": np.random.normal(50, 10, n_samples),
    "Feature2": np.random.exponential(10, n_samples),
    "Feature3": np.random.uniform(0, 10, n_samples),
    "Feature4": np.random.binomial(1, 0.3, n_samples)
})

# Introduz dados ausentes
X_missing = X.copy()
mask = np.random.rand(*X.shape) < 0.2
X_missing[mask] = np.nan

# Aplica 3 métodos de imputação: média, knn, random forest mice
mean_imputer = SimpleImputer(strategy="mean")
X_mean = pd.DataFrame(mean_imputer.fit_transform(X_missing), columns=X.columns)

knn_imputer = KNNImputer(n_neighbors=5)
X_knn = pd.DataFrame(knn_imputer.fit_transform(X_missing), columns=X.columns)

rf_mice = IterativeImputer(estimator=RandomForestRegressor(
    n_estimators=10, random_state=42), max_iter=10, random_state=42)
X_rf_mice = pd.DataFrame(rf_mice.fit_transform(X_missing), columns=X.columns)

# Visualização dos Resultados
sns.set_theme(style="whitegrid")
fig, axes = plt.subplots(2, 2, figsize=(14, 10))
fig.suptitle('Impacto das Diferentes Imputações nas Distribuições (20% de NaNs)', fontsize=16)

# Função auxiliar para desenhar as curvas de cada coluna sobrepostas
def plot_comparacao(ax, feature_name, title):
    # Linha preta grossa: A verdade absoluta (dados originais sem buracos)
    sns.kdeplot(data=X, x=feature_name, ax=ax, label='Original (Real)', color='black', linewidth=3)
    
    # Linhas coloridas: As tentativas de reconstrução
    sns.kdeplot(data=X_mean, x=feature_name, ax=ax, label='Média', color='red', linestyle='--')
    sns.kdeplot(data=X_knn, x=feature_name, ax=ax, label='KNN', color='blue', linestyle='-.')
    sns.kdeplot(data=X_rf_mice, x=feature_name, ax=ax, label='Random Forest (MICE)', color='green', linestyle=':')
    
    ax.set_title(title, fontsize=12)
    ax.set_ylabel('Densidade')
    ax.legend()

# Aplicando a função para cada Feature nos 4 quadrantes do gráfico
plot_comparacao(axes[0, 0], 'Feature1', 'Feature 1 (Normal)')
plot_comparacao(axes[0, 1], 'Feature2', 'Feature 2 (Exponencial)')
plot_comparacao(axes[1, 0], 'Feature3', 'Feature 3 (Uniforme)')
plot_comparacao(axes[1, 1], 'Feature4', 'Feature 4 (Binomial)')

# Na representação desses dados, o eixo X mostra os números reais dos dados, e o
# eixo Y mostra a concentração desses dados, a aglomeração dos dados naquele valor.

plt.tight_layout()
plt.show()