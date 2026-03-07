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
print(cep_to_idx)