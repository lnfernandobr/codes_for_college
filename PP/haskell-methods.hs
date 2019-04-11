----      Função Para somar Numeros        ----
----   Ex: soma 4 = 1 + 2 + 3 + 4 = 10     ----
soma 1 = 1
soma 0 = 0
soma n = soma(n-1) + n
-----------------------------------------------



----            Fatorial                   ----
----   Ex: fat 5 = 5 * 4 * 3 * 2 * 1 = 120 ----
fat 1 = 1
fat 0 = 1
fat n = fat(n-1) * n
-----------------------------------------------



----            Fibonacci                   ----
----       Ex: 1 1 2 3 5 8 13               ----
fib 0 = 0
fib 1 = 1
fib n = fib (n-1) + fib (n-2) 
-----------------------------------------------
 
 

----            GUARDA                     ----
guarda x | (x == 0) = 0
         | (x == 1) = 1
         | otherwise = 10
-----------------------------------------------



----            Function AND               ----
myand :: Bool -> Bool -> Bool
myand False _ = False
myand _ False = False 
myand True True = True
-----------------------------------------------


----       TUPLA  Ex: (1,2,3,4,5)       ----
func :: (Int, Int) -> (Int, Int) -> (Int, Int)
func (a, b) (c, d) = (a + c, b + d) 
-----------------------------------------------




----       Extrair dados de uma Tupla      ----
-- fst -> extrai o primeiro elemento de uma tupla
-- snd -> extrai o segundo elemento de uma tupla

nomes :: (String, String, String) 
nomes = ("Marcos", "GeeksBrs", "Haskell")
selec_prim (x, _, _) = x
selec_sec (_, y, _) = y
selec_ter (_, _, z) = z

-----------------------------------------------




----    Novos tipos, FUNÇÃO TYPE          ----
type Nome = String
type Idade = Int
type Linguagem = String

type Pessoa = (Nome, Idade, Linguagem)
pessoa :: Pessoa
pessoa = ("Fernando", 20, "Javascript")


my_fst :: Pessoa -> Nome
my_fst (n, _, _) = n
-----------------------------------------------





------------                LISTAS           --------------------
-- head -> Retorna a cabeca da lista
-- tail -> Retorna o corbo da lista (retorna tudo Menos a cabeça)
-- : -> Adiciona numa lista 1: [2] = [1, 2]

-------------------------------------------------------------------




-- Comprimento de uma lista 
size_list [] = 0
-- x cabeca da lista e xs o resto da lista menos a cabeça
size_list (x:xs) = 1 + size_list xs



-- Função que verifica se duas listas são iguais
  -- * Mesmo numero de elementos
  -- * Mesmos elementos nas mesmas posições
comp_listas ::  [Int] -> [Int] -> Bool
comp_listas [] [] = True
comp_listas [] _ = False
comp_listas _ [] = False
comp_listas (a:b) (c:d) | (a == c) = comp_listas b d
                        | otherwise = False




-- Função que recebe uma lista e retorna o invers dessa lista 
  -- Ex: [1, 2, 3] ---> [3, 2, 1]

inv_aux :: [t] -> [t] -> [t]
inv_aux [] l_inv = l_inv
inv_aux (x:xs) l_inv = inv_aux xs l_inv++[x]

inv_lista :: [t] -> [t]
inv_lista [] = []
inv_lista l = inv_aux l []
  


-- Função que recebe uma lista e retorna o invers dessa lista 
  -- Ex: [1, 2, 3] ---> [3, 2, 1]
  ------------------ 2 MODO ------------------------
  
inv_lista2 :: [t] -> [t]
inv_lista2 [] = []
inv_lista2 (x:xs) = inv_lista2 xs ++ [x]




-- Função que verirfica se um numero n percente a lista
percente :: [Int] -> Int -> Bool
percente [] _ = False
percente (x:xs) n | (x == n) = True
                  | otherwise = percente xs n


-- Funçao que verifica o maior elemento da lista
maior :: [Int] -> Int
maior [x] = x
maior (x:xs) | (x > maior xs) = x
             | otherwise = maior xs


-- Função que veririca se todos os elementos da lista são pares
todos_pares :: [Int] -> Bool
todos_pares [] = True
todos_pares (x:xs) | (mod x 2 /= 0) = False 
                   | otherwise = todos_pares xs



main = do print(todos_pares [2,4, 6, 12])




par :: Int -> Bool
par x = mod x 2 == 0

lista = [x | x <- [1 .. 10]]

tuplas = [(x, y) | x <- [1..5], y <- [6..10], par x, par y]

main = print(tuplas)



-- Função zip
  -- zip :: [a] -> [b] -> [(a, b]

lista :: [Int]
lista = [5,2,10,1,9]



-- Ordenação de Listas
  -- Entrada = [5,8,1,10]
  -- Saida = [1,5,8,10]
  -- [5 ,8, 1, 10] []
  -- [5 ,8, 10] [1]
  -- [8, 10] [1, 5]
  -- [10] [1, 5, 8]
  -- [] [1, 5, 10]
  
get_menor :: [Int] -> Int
get_menor [] = 0
get_menor [x] = x
get_menor (x:xs) | (x < get_menor xs) = x
                 | otherwise = get_menor xs

remove_menor :: [Int] -> [Int]
remove_menor [] = []
remove_menor (x:xs) | (x == (get_menor (x:xs) ) ) = xs
                    | otherwise = (x: remove_menor xs)

 
aux_ordena :: [Int] -> [Int] -> [Int]
aux_ordena lista_ordenada [] = lista_ordenada
aux_ordena lista_ordenada (x:xs) = 
  aux_ordena (lista_ordenada++[get_menor(x:xs)]) (remove_menor (x:xs))
 
 
ordena :: [Int] -> [Int]
ordena [] =[]
ordena lista = aux_ordena [] lista
 
--main = print(ordena lista)


-- Função para inverter lista

inverte :: [a] -> [a]
inverte [] = []
inverte (x:xs) = (inverte xs)++[x]


--main = print(inverte ["a, "c", "b"])




-- If em Haskell

if_par :: Int -> Bool
if_par n = if(mod n 2 == 0) then True else False

--main = print(if_par 24789322)




-- Função Numero Primo
fatores 1 = [1,1]
fatores n = [i|i<-[1..90], n `mod` i == 0]

primo l = [i|i<-l,fatores i == [1,i]]

--main = do
  print(primo [1..1000])


--Função Verifica par e primo ordenados

quicksort::(Ord a) => [a] -> [a]
quicksort [] = []
quicksort (s:xs) = quicksort [x|x <- xs,x < s] ++ [s] ++ quicksort [x|x <- xs,x >= s]

par l = [x | x <- l, x `mod` 2 == 0]
impar l = [x | x <- l, x `mod` 2 /= 0]

separaImparPar l = (quicksort(impar l),quicksort(par l))

ordenaImparPar l = quicksort(impar l)++quicksort(par l)

--main = do
  print(separaImparPar [8,7,6,5,1,2,3,4])
  print(ordenaImparPar [8,7,6,5,1,2,3,4])



-- Fibonacci em Lista

fibonacci :: (Int) -> Int
fibonacci (x)
  |x == 0 = 0
  |x == 1 = 1
  |otherwise = fibonacci(x-1)+fibonacci(x-2)

    fibonacciList x = map fibonacci [0 .. x]

--main = print(fibonacciList 5)




--- FUNÇÕES UTEIS NO HASKELL
    -- null [] Verifica se a lista ta vazia
    -- last [] devolve o ultimo elemento da lista
    -- init [] retorna lista, menos o ultimo elemento
    -- head [] retorna a cabeca da lista
    -- length [] retorna o tamanho da lista
    -- drop n [] Apaga os n primeiros elementos
    -- take n [] retorna os n primeiros elementos em forma de lista
    -- reverse [] retorna o reverso da lista
    -- splitAt 2 "algumaCoisa" -> Retorna ("al", "gumaCoisa") Corta a String
    -- zip [] [] Retorna uma lista de tuplas, EX: zip [1,2,3] ['a', 'b', 'c'] ->   [ (1,'a'), (2,'b'), (3,'c') ]
    -- unzip [ (1,'a'), (2,'b'), (3,'c') ]   Retorna -> ([1,2,3],"abc")


