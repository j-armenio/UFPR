module PrimeiroPrograma where

-- Base de dados
num :: Int -> Float
num 1 = 5.0
num 2 = 10.0
num 3 = 15.0
num 4 = 20.0
num 5 = 25.0

-- Função recursiva de soma
soma :: Int -> Float
soma 0 = 0
soma n = num n + soma (n-1)

-- Função de média
media :: Int -> Float
media n = (soma n) / (fromIntegral n)
