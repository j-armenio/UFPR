module Lista4 where

-- Ex.1

getFst :: (String, String, Char) -> String
getFst (a, _, _) = a

getSnd :: (String, String, Char) -> String
getSnd (_, b, _) = b

getTer :: (String, String, Char) -> Char
getTer (_, _, c) = c

-- Ex.2

base :: Int -> (String, String, Char)
base x
  | x == 1 = ("joao", "mestre", 'm')
  | x == 2 = ("jonas", "doutor", 'm')
  | x == 3 = ("joice", "mestre", 'f')
  | x == 4 = ("janete", "doutor", 'f')
  | x == 5 = ("jocileide", "doutor", 'f')
  | otherwise = ("ninguem", "", 'x')

contMestre :: Int -> Int
contMestre 0 = 0
contMestre n
  | getSnd (base n) == "mestre" = 1 + contMestre (n - 1)
  | otherwise = contMestre (n - 1)

contDoc :: Int -> Int
contDoc 0 = 0
contDoc n
  | getSnd (base n) == "doutor" = 1 + contDoc (n - 1)
  | otherwise = contDoc (n - 1)

contMD :: String -> Int -> Int
contMD _ 0 = 0
contMD str n
  | str == getSnd (base n) = 1 + contMD str (n - 1)
  | otherwise = contMD str (n - 1)

cont :: String -> Char -> Int -> Int
cont _ _ 0 = 0
cont str g n
  | str == getSnd (base n) && g == getTer (base n) = 1 + cont str g (n-1)
  | otherwise = cont str g (n-1)

-- Ex.3

type Nome = String
type Titulo = String
type Genero = Char
type Pesquisador = (Nome, Titulo, Genero)
type Grupo = [Pesquisador]

base2 :: Int -> (Pesquisador)
base2 x
  | x == 1 = ("joao", "mestre", 'm')
  | x == 2 = ("jonas", "doutor", 'm')
  | x == 3 = ("joice", "mestre", 'f')
  | x == 4 = ("janete", "doutor", 'f')
  | x == 5 = ("jocileide", "doutor", 'f')
  | otherwise = ("ninguem", "", 'x')

criaGrupo :: Int -> Grupo
criaGrupo 0 = []
criaGrupo n = base n : criaGrupo (n-1)

doutores :: Grupo -> [Nome]
doutores grupo = [ (nome) | (nome, titulo, _) <- grupo, titulo == "doutor" ]

-- Ex.4

addSr :: String -> String
addSr nome = "Sr." ++ nome

addSra :: String -> String
addSra nome = "Sra." ++ nome

addSrta :: String -> String
addSrta nome = "Srta." ++ nome

mapNome :: (String -> String) -> [String] -> [String]
mapNome _ [] = []
mapNome f (h:t) = (f h) : (mapNome f t)

-- Ex.5

ehPositivo :: Float -> Bool
ehPositivo n = n > 0

ehNegativo :: Float -> Bool
ehNegativo n
  | n < 0 = True
  | otherwise = False

ehZero :: Float -> Bool
ehZero n
  | n == 0 = True
  | otherwise = False

filtraNum :: (Float -> Bool) -> [Float] -> [Float]
filtraNum _ [] = []
filtraNum f (h:t)
  | f h == True = h : (filtraNum f t) 
  | otherwise = filtraNum f t

