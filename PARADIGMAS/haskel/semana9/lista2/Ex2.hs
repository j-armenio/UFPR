module Ex2 where

fibonacci :: Int -> Int
fibonacci n
  | n == 0 = 0
  | n == 1 = 1
  | n > 1 = fibonacci (n-1) + fibonacci (n-2)
  | otherwise = -1

digitos :: Int -> Int
digitos n
  | n < 10 = 1
  | otherwise = 1 + digitos (div n 10)

somaDig :: Int -> Int
somaDig n
  | n == 0 = 0
  | otherwise = n `mod` 10 + somaDig (n `div` 10)
