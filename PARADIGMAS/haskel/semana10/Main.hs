module Main where
import Lista4

main :: IO ()
main = do
  let nomes = ["Joao", "Carlos", "Pedro"]
  let nomesSr = mapNome addSrta nomes
  print nomesSr

  let numeros = [1, 2, -3, 4, -5]
  let numerosPositivo = filtraNum ehPositivo numeros
  print numerosPositivo
  