import torch
import torch.nn as nn

# Dataset de código postal
cep_list = [
    '01310-100', '20040-002', '30140-071', '80010-000', '70040-010', 
    '40010-000', '50010-000', '60060-000', '90010-001', '01001-000', 
    '22041-001', '80240-000', '69010-000', '74015-010', '59020-000', 
    '66010-000', '78010-000', '65010-000', '57020-000', '29010-000',
]

# Faz uma lista só com ceps não repetidos
unique_ceps = list(set(cep_list))
# enumera os ceps
cep_to_idx = {cep_code: idx for idx, cep_code in enumerate(unique_ceps)}
# pega a lista e transforma em um Tensor (estrutura otimizada de ML) 
cep_indices = torch.tensor([cep_to_idx[cep_code] for cep_code in cep_list])

# define tamanho do vetor
embedding_dim = 10
# cria a Lookup Table/Camada de Embedding, onde cada linha representa um CEP e
# cada coluna uma das 10 dimensões
embedding = nn.Embedding(num_embeddings=len(unique_ceps), embedding_dim=embedding_dim)

# gera os embeddings
cep_embeddings = embedding(cep_indices)

print(cep_embeddings)