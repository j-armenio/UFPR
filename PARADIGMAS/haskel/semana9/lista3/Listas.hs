module Listas where

soma :: [Int] -> Int
soma [] = 0
soma (h:t) = h + soma t

removePares :: [Int] -> [Int]
removePares [] = []
removePares (h:t)
  | h `mod` 2 == 0 = removePares t
  | otherwise = h : removePares t

inverte :: [Float] -> [Float]
inverte [] = []
inverte (h:t) = inverte t ++ [h]

nomesA :: [String] -> [String]
nomesA [] = []
nomesA (h:t)
  | head h == 'A' = h : nomesA t
  | otherwise = nomesA t

