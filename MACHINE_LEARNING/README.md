# Inicializar ambiente

1. Cria o ambiente.
```
conda env create -f environment.yml
```

2. Ativa o ambiente.
```
conda activate ml_ufpr
```

---

# Configurar

- Instalar novo pacote.
```
conda install nome_pacote
```

- Sobreescrever arquivo de ambiente.
```
conda env export --from-history > environment.yml
```
