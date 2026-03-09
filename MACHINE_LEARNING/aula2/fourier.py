import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report
from skfda.representation import FDataGrid
from skfda.representation.basis import FourierBasis

# carrega e organiza o dataset ECG5000
data = pd.read_csv('media/ECG5000.txt', header=None)
X = data.iloc[:, :-1].values
y_raw = data.iloc[:, -1].values

# Transforma os dados em binarios, 1=normal, 0=anormal
y = np.array([1 if i==1 else 0 for i in y_raw])

# Cria um objeto FDataGrid: objeto que ve os dados como uma curva continua
fd = FDataGrid(data_matrix=X, grid_points=np.linspace(0, 1, X.shape[1]))

# Define a Base de Fourier
n_basis = 13
fourier_basis = FourierBasis(domain_range=(0, 1), n_basis=n_basis)
fd_basis = fd.to_basis(fourier_basis)

# extrai os coeficientes para classificação
X_features = fd_basis.coefficients
X_train, X_test, y_train, y_test = train_test_split(X_features, y, test_size=0.2, random_state=42)
clf = RandomForestClassifier(n_estimators=100, random_state=42)
clf.fit(X_train, y_train)
y_pred = clf.predict(X_test)
acc = accuracy_score(y_test, y_pred)

print(f"Classification accuracy using Fourier basis: {acc:.4f}")
print(classification_report(y_test, y_pred))