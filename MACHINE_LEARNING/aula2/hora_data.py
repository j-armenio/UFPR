import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# cria um DataFrame simulando coluna de horas, de 0 a 23
df = pd.DataFrame({'hour': range(24)})

# aplica formula
df['hour_sin'] = np.sin(2 * np.pi * df['hour'] / 24)
df['hour_cos'] = np.cos(2 * np.pi * df['hour'] / 24)

plt.figure(figsize=(8,8))

# cria o gráfico de dispersão
# eixo x recebe o cos, eixo y recebo o sen
scatter = plt.scatter(df['hour_cos'], df['hour_sin'], c=df['hour'], cmap='twilight', s=100) 

# A aplicação dessas formulas, e depois posicionar os valores nos eixos x e y vai
# mapear a posição no espaço das horas como um "circulo", fazendo com que horas
# proximas logicamente estejam proximas espacialmente.

# escreve numeros da horas 
for i in range(24):
    plt.annotate(f"{i}h", 
                 (df['hour_cos'][i], df['hour_sin'][i]), 
                 xytext=(df['hour_cos'][i] * 1.15, df['hour_sin'][i] * 1.15), 
                 ha='center', va='center', fontsize=11, fontweight='bold')

# ajustes estéticos
plt.title('Representação Cíclica das 24 Horas', fontsize=16, pad=20)
plt.xlabel('hour_cos (Eixo X)', fontsize=12)
plt.ylabel('hour_sin (Eixo Y)', fontsize=12)
plt.axhline(0, color='gray', linestyle='--', linewidth=1)
plt.axvline(0, color='gray', linestyle='--', linewidth=1)
plt.gca().set_aspect('equal')
plt.xlim(-1.4, 1.4)
plt.ylim(-1.4, 1.4)
plt.colorbar(scatter, label='Evolução das Horas', shrink=0.7)
plt.grid(True, alpha=0.3)

plt.show()
