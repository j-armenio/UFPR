import pandas as pd
from sklearn.feature_extraction import FeatureHasher

# Dataset de código postal
cep_list = [
    '01310-100', '20040-002', '30140-071', '80010-000', '70040-010', 
    '40010-000', '50010-000', '60060-000', '90010-001', '01001-000', 
    '22041-001', '80240-000', '69010-000', '74015-010', '59020-000', 
    '66010-000', '78010-000', '65010-000', '57020-000', '29010-000',
]

# Cria um DataFrame (tabela do Pandas) passando o dicionario como entrada
# CEP será o nome da coluna, cep_list preenchera com os dados
# CEP
# 01310-100
# 20040-002
# 30140-071
# ...

data = pd.DataFrame({'CEP': cep_list})

# List Comprehension: transforma dados da coluna em uma lista de dicionarios
#           [item          for item     in interval]
# Para cada CEP encontrado ele cria um dicionário
# [
#   {'01310-100': 1},
#   {'20040-002': 1}
# ]
zip_dicts = [{cep_code: 1} for cep_code in data['CEP']]

# Cria e configura o hasher
# n_features: tamanho do array de saida
# input_type: formato dos dados de entrada
# alternate_sign: False garante valores resultantes positivos
hasher = FeatureHasher(n_features=10, input_type='dict', alternate_sign=False)

# Percorre cada dicionário da lista, para cada CEP:
# 1. aplica hash no texto do CEP
# 2. aponta para cada um dos 10 indices (buckets)
# 3. marca aquele indice como valor encontrado no dicionario
hashed_features = hasher.transform(zip_dicts).toarray()

# Mostra como DataFrame
hashed_df = pd.DataFrame(hashed_features, columns=[f'hash_{i}' for i in range(10)])
result_df = pd.concat([data, hashed_df], axis=1)

# Mostra exemplo
print(result_df.head(15))
