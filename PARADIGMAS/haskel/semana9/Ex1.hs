module Ex1 where

deMaior :: Int -> String
deMaior n = 
  if n >= 18
    then "Pode"
    else "Não pode"

deMaiorG :: Int -> String
deMaiorG n
  | n >= 18 = "Pode"
  | otherwise = "Não pode"

ehPositivo :: Int -> String
ehPositivo n = 
  if n == 0
    then "Igual a zero"
  else if n > 0
    then "Eh positivo"
  else "Eh negativo"

ehPositivoG :: Int -> String
ehPositivoG n
  | n == 0 = "Igual a zero"
  | n > 0 = "Eh positivo"
  | otherwise = "Eh negativo"

bissexto :: Int -> Bool
bissexto n =
  if (mod n 4 == 0 && mod n 100 /= 0) || mod n 400 == 0
    then True
  else False

bissextoG :: Int -> Bool
bissextoG n
  | (mod n 4 == 0 && mod n 100 /= 0) || mod n 400 == 0 = True
  | otherwise = False