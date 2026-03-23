import pandas as pd
from skfda.representation import FDataGrid
from skfda.preprocessing.dim_reduction import FPCA  
from sklearn.model_selection import train_test_split 
from sklearn.ensemble import RandomForestClassifier 
from sklearn.metrics import classification_report 

# carrega e organiza o dataset ECG5000
data = pd.read_csv('../datasets/ECG5000.txt', header=None, sep=r'\s+')

y = data.iloc[:, 0].values   
X = data.iloc[:, 1:].values  

# Transforma os dados em binarios, 1=normal, 0=anormal
y = [1 if i == 1 else 0 for i in y]

# Agora o FDataGrid vai ler os arrays NumPy corretamente
fd = FDataGrid(X)

# Inicializa FPCA com número de componentes desejado
n_components = 3
fpca = FPCA(n_components=n_components)
fpca.fit(fd)

# Transforma os dados para obter os scores do componentes principais
X_fpca = fpca.transform(fd)

# Divisão de treino e teste
X_train, X_test, y_train, y_test = train_test_split(X_fpca, y, test_size=0.2, random_state=42)

# Treinamento do classificador
clf = RandomForestClassifier(n_estimators=100, random_state=42)
clf.fit(X_train, y_train)

# Predição e métricas
y_pred = clf.predict(X_test)
print(classification_report(y_test, y_pred))