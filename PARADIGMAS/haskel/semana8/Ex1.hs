module FuncoesDiversas where

reajuste :: Float -> Float
reajuste x = x * 1.25

mediaPonderada :: Float -> Float -> Float -> Float -> Float -> Float -> Float
mediaPonderada x y z a b c = (x * a + y * b + z * c) / a + b + c

converterTemperatura :: Double -> Double
converterTemperatura x = x * (9.0 / 5.0) + 32.0

calcularIdade :: Int -> Int
calcularIdade x = 2032 - x

calcularMinutos :: Int -> Int -> Int
calcularMinutos x y = x * 60 + y

somaQuadrados :: Int -> Int -> Int -> Int
somaQuadrados x y z = x * x + y * y + z * z

somaQuadrados1 :: Int -> Int -> Int -> Int
somaQuadrados1 x y z = x ^ (2 :: Int) + y ^ (2 :: Int) + z ^ (2 :: Int)

somaQuadrados2 :: Float -> Float -> Float -> Float
somaQuadrados2 x y z = x**2 + y**2 + z**2

impar :: Int -> Bool
impar x = mod x 2 /= 0

impar1 :: Int -> Bool
impar1 x = not (even x)

divisibilidade :: Int -> Int -> Bool
divisibilidade x y = mod x y == 0

salario :: Float -> Float
salario x = x + (x * 0.05) - (x * 0.07)