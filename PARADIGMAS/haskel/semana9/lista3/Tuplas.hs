module Tuplas where

vai :: Int -> (Int, Int, Int, Int)
vai x = (x*2, x*3, x*4, x*5)

sla :: Int -> (Int, String)
sla n1 = (n1 `div` 2, value)
  where 
    value = if n1 `mod` 2 == 0
              then "Par"
              else "Impar"

somaProd :: [Int] -> (Int, Int)
somaProd [] = (0, 1)
somaProd (h:t)
  | h > 0 = (soma + h, prod)
  | h < 0 = (soma, prod * h)
  | otherwise = (soma, prod)
  where
    (soma, prod) = somaProd t

type Nome = String
type Nota = Int
type Tipo = String
type PontoTuristico = (Nome, Nota, Tipo)

turi :: Int -> [PontoTuristico] -> [(Nome, Nota)]
turi notaReq pontos =
  [ (nome, nota) | (nome, nota, tipo) <- pontos, nota > notaReq, tipo == "parque" || tipo == "museu" ]

