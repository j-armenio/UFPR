# Problema: Colorir mapa

O problema consiste em:

1. Cada região em um mapa deve ser colorida.
2. Nenhuma região adjacente pode ter a mesma cor.
3. Cores: **vermelho**, **verde** ou **azul**.

## Mapa da Australia

### Estruturar o problema de CSP

Definir

* **Variáveis**: Cada região do mapa será uma variável que precisa de uma cor.
X = {WA, NT, SA, Q, NSW, V, T}

* **Domínio**: O conjunto de cores possíveis para cada região.
D = {red, green, blue}

* **Restrições**: As restrições entre as variáveis, ou seja, nenhuma região adjacente pode ter a mesma cor.
C = {SA!=WA, SA!=NT, SA!=Q, SA!=NSW, SA!=V, WA!=NT, NT!=Q, Q!=NSW, NSW!=V}

### Entradas:

* Australia

WA, NT, SA, Q, NSW, V, T
red, green, blue
('SA','WA'), ('SA','NT'), ('SA','Q'), ('SA','NSW'), ('SA','V'), ('WA','NT'), ('NT','Q'), ('Q','NSW'), ('NSW','V')

* Brasil


