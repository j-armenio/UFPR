import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split 
from sklearn.preprocessing import MinMaxScaler
from tensorflow.keras.layers import Input, Dense
from tensorflow.keras.models import Model
from tensorflow.keras.optimizers import Adam

# carrega e organiza o dataset ECG5000
data = pd.read_csv('media/ECG5000.txt', header=None, sep=r'\s+')

y = data.iloc[:, 0].values   
X = data.iloc[:, 1:].values  

# Transforma os dados em binários, 1=normal, 0=anormal
y = [1 if i == 1 else 0 for i in y]

# Divisão de treino e teste ANTES de normalizar
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42)

# Normaliza os dados (fit_transform no treino, apenas transform no teste)
scaler = MinMaxScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Define as dimensões do autoencoder
input_dim = X_train_scaled.shape[1]
encoding_dim = 7 # Tamanho do espaço latente (bottleneck)

# Construção do Autoencoder
input_signal = Input(shape=(input_dim,))

# Encoder (compressão)
encoded = Dense(64, activation='relu')(input_signal)
encoded = Dense(encoding_dim, activation='relu')(encoded)

# Decoder (reconstrução)
decoded = Dense(64, activation='relu')(encoded)
decoded = Dense(input_dim, activation='sigmoid')(decoded)

# Compilação do modelo
autoencoder = Model(input_signal, decoded)
autoencoder.compile(optimizer=Adam(learning_rate=0.001), loss='mse')

# Treinamento
autoencoder.fit(X_train_scaled, X_train_scaled,
                epochs=50,
                batch_size=32,
                shuffle=True,
                validation_data=(X_test_scaled, X_test_scaled),
                verbose=0) 
                
# Cria o modelo isolado apenas para o Encoder
encoder = Model(input_signal, encoded)

# Extrai as características reduzidas (espaço latente)
X_train_latent = encoder.predict(X_train_scaled)
X_test_latent = encoder.predict(X_test_scaled)

print(f"Formato original: {X_train_scaled.shape}")
print(f"Formato reduzido (latente): {X_train_latent.shape}")