module Compreensao where

-- [0, 3, 6, 9, 12, 15]
multTres :: [Int]
multTres = [ 3*x | x <- [0..5]]

-- [[1],[2],[3],[4],[5]]
list :: [[Int]]
list = [ [x] | x <- [1..5]]