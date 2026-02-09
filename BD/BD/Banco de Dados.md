# Modelo relacional

**Modelo relacional** é composto por:
- Conjunto de objetos e relações
- Conjunto de operadores para agir sobre relações
- Integridade de dados para precisão e consistência

**Chave primária (PK)**: identifica uma linha de dados de uma tabela;
**Chave estrangeira (FK)**: relaciona logicamente os dados de várias tabelas.

É acessado por **SQL** (Structured Query Language).

---
# Postgres psql

**Acesso**: psql -U ci218 -h bd.c3sl.ufpr.br -d tpch

`\d` Lista as tabelas da base de dados
`\d nome_tabela` Descreve todos atributos da tabela e suas propriedades
`\l` Lista o nome, dono e codificação
`\q` Sai do psql

---
# Instruções SQL

- Não tem distinção entre maiúscula e minúscula.
- Podem estar em uma ou mais linhas.

## Instruções estruturais
Comandos que definem a estrutura do banco.

#### CREATE
Cria um objeto novo do zero.
```sql
CREATE TABLE Produtos (
	ID INT PRIMARY KEY,
	Nome VARCHAR(100),
	Preco DECIMAL(10, 2)
);
```

#### ALTER
Modifica estrutura de algo que já existe.
```sql
ALTER TABLE Produtos ADD Estoque INT;
```

#### RENAME
Muda o nome de uma tabela ou coluna.
```sql
ALTER TABLE Produtos RENAME TO Mercadorias;
```

#### TRUNCATE
Apaga todos os dados da tabela, mas mantém a estrutura (as colunas) intactas.
```sql
TRUNCATE TABLE Mercadorias;
```

#### DROP
Apaga a tabela inteira do banco de dados.
```sql
DROP TABLE Mercadorias;
```

## Instruções de dados

#### INSERT
Adiciona novos registros (linhas) dentro de uma tabela existente. 
```sql
INSERT INTO Produtos (ID, Nome, Preco)
VALUES (1, 'Teclado Mecânico', 250.00);
```

#### UPDATE
Modifica dados que já estão gravados.
- Sempre use a clausula `WHERE`, se não vai mudar todas linhas da tabela.
```sql
UPDATE Produtos
SET Preco = 300.00
WHERE ID = 1;
```

#### SELECT básico
**SELECT**: identifica que colunas
**FROM**: identifica qual tabela
```sql
SELECT coluna1, coluna2
FROM tabela;
```

Seleciona todas colunas:
```sql
SELECT * FROM region;
```

#### Expressões Aritméticas
```sql
SELECT p_name, p_size, p_size+100 FROM part;     -- + soma
SELECT p_name, p_size, 2*(p_size+100) FROM part; -- * multiplicacaoo
SELECT p_name, p_size, 2*(p_size+100) AS calc FROM part; -- AS da um apelido a coluna
SELECT p_name || p_type AS newCol form part; -- || concatenacao
SELECT p_name || ' is a ' || p_type as       -- da pra concatenar com strings
```
*Strings de caracteres e data* sempre entre aspas simples `''`, ex: `BRAZIL` e  `'1996-03-13'`.

#### DISTINCT
Elimina linhas duplicadas.
```sql
SELECT DISTINCT c_mktsegment FROM customer;
```

#### WHERE
Limita linhas selecionadas.
```sql
SELECT coluna1, coluna2,
FROM tabela
WHERE condicao;

-- Retorna relacao com linhas onde n_regionkey eh igual 0
SELECT * FROM
FROM nation
WHERE n_regionkey=0;
```

#### Operadores de Comparação
`=` Igual a
`>` Maior do que
`>=` Maior ou igual a
`<` Menor que
`<=` Menor ou igual a
`<>` Diferente de
`BETWEEN` Faixa de valores
`IN` Filtra valor de um campo a partir de uma lista de possibilidades
`LIKE` Buscar por uma determinada string dentro de um campo com valores textuais
`IS NULL` verifica valores que são nulos, valor não disponível

```sql
SELECT l_linestatus, l_shipdate
FROM lineitem
WHERE l_shipdate >= '1998-11-30';

SELECT p_name, p_size
FROM part
WHERE p_size BETWEEN 3 AND 5

SELECT p_name, p_size
FROM part
WHERE p_size IN (2,9,11);

SELECT p_name, p_type
FROM part
WHERE p_type LIKE '%PROMO'; -- '%' é um curinga

SELECT r_name 
FROM region 
WHERE r_name LIKE '_M%'; -- '_' é um curinga de exatamente um caractere

SELECT r_name
FROM region
WHERE r_name IS NULL;
```

#### Operadores lógicos
`AND` Verdadeiro se condições de componentes forem verdadeiras
`OR` Uma condição ou a outra podem ser verdadeiras
`NOT` Retorna campos sem a condição

```sql
SELECT p_name, p_type
FROM part
WHERE p_type NOT LIKE '%ECONOMY%';
```

#### ORDER BY
Classifica as linhas em `ASC`(crescente) e `DESC`(decrescente).
```sql
SELECT n_name 
FROM nation
ORDER BY n_name DESC;
```

### Funções SQL
Executam cálculos, modificam itens, manipula saídas, formata datas e números e converte tipos.
- É possível aninhar funções.
#### Funções de datas
`age(timestamp, timestamp)` Calcula idades
`current_date` Data atual
`current_time` Hora atual
`current_timestamp` Hora e data atual
`to_date(date, 'YYYYMMDD')` Converte string para formato data

```sql
SELECT age('2017-01-01','2011-06-24'); -- saida: 5 years 6 mons 7 days
SELECT age(timestamp '2000-01-01'); -- calcula com base na current_date

SELECT to_date('2018-08-25', 'YYYYMMDD'); -- output: 2018-08-25
```

#### Funções numéricas
`abs()` Calcula valor absoluto de um número
`ceil()` Menor inteiro não menor que o argumento (teto)
`floor()` Maior inteiro não maior que o argumento (piso)
`mod()` Resto de y/x
`sqrt()`
`round()` Arredonda um valor para o inteiro mais próximo
`trunc()` Retorna um número truncado para um inteiro

```sql
SELECT round(10.817, 2); -- saida: 10.82
```

#### Funções de caracteres
`concat()` Concatena duas ou mais strings
`format()` Formata argumentos baseados em uma string de formato
`length()` Retorna o número de caracteres ou bytes de uma string
`replace()` Substitui todas as ocorrências de uma string

```sql
SELECT concat(p_name, ' ', p_type) AS newName 
FROM part;

SELECT format('NOME: %s, TIPO: %s', p_name, p_type) as newName
FROM part;

SELECT r_name, replace(r_name, 'A', '0')
FROM region; -- vai ter uma coluna replace com strings com '0' no lugar de 'A'
```

#### Funções de conversão
`to_number()` caracter -> number
`to_date()`    caracter -> date
`to_char()`    any -> caracter

#### Funções de grupo
É proibido usar funções na clausula `WHERE`, nesse caso use `HAVING`.
```sql
SELECT c.c_mktsegment
FROM customer c
GROUP BY c.c_mktsegment
HAVING AVG(c.c_acctbal)>4500;
```

### Exibindo dados de várias tabelas
#### Produto cartesiano
Todas linhas da primeira e segunda tabela estão unidas.
- Quando uma condição de junção estiver omitida/inválida. 

```sql
SELECT region.r_name, nation.n_name
FROM region, nation;
```

#### Junção
Usada para consultar dados de uma ou mais tabelas.
- É necessário criar uma **condição** de junção na cláusula `WHERE`, utilizando prefixo da tabela para indicar de qual tabela é a coluna.

```sql
SELECT tabela1.coluna, tabela2.coluna
FROM tabela1, tabela2
WHERE tabela1.coluna1 = tabela2.coluna2

SELECT region.r_name, nation.n_name
FROM region, nation
WHERE region.r_regionkey = nation.n_regionkey 
AND region.r_name='AFRICA';
	
-- É possível unir mais de duas tabelas
SELECT r.r_name, n.n_name, c.c_name
FROM region r, nation n, customer c
WHERE r.r_regionkey = n.n_regionkey
AND c.c_nationkey = n.n_nationkey
AND r.r_name = 'AFRICA';

-- Junção NÃO-IDÊNTICA: nenhuma coluna das tabelas correspondem
SELECT p.p_brand, o.o_orderstatus
FROM part p, orders o
WHERE p.p_retailprice > o.o_totalprice
LIMIT 10;

-- AutoJunções: unindo uma tabela a ela mesma
SELECT l_orderkey, l_shipdate, l_commitdate
FROM lineitem
WHERE l_shipdate = l_commitdate
LIMIT 10;
```

##### LEFT  OUTER JOIN
*Ex*: Todas regiões incluindo regiões que não tem país
```sql
SELECT r.r_name, n.n_name
FROM region r LEFT OUTER JOIN nation n
ON (r.r_regionkey = n.n_regionkey)
```
saída: todas regiões somadas com seus países (informação da tabela nation)
- RIGHT JOIN é a mesma coisa só que muda o sentido.

##### INNER JOIN
*Ex*: Para cada linha da tabela region queremos os países correspondentes
```sql
SELECT r.r_name, n.n_name
FROM region r INNER JOIN nation n
ON (r.r_regionkey = n.n_nationkey);
```
- É igual a um join normal.

##### CROSS JOIN
*Ex*: Para cada linha da tabela region queremos todas as linhas da tabela nation ou vice-versa.
```sql
SELECT r.r_name, n.n_name
FROM region r CROSS JOIN nation n;
```
* É igual ao produto cartesiano.

 que você digita para produzir esse resultado propositalmente.

### Funções de Agregação
Operam em conjuntos de linhas para fornecer um resultado por grupo.
- ignoram valores nulos, com exceção do `count`.

`AVG` Média
`SUM` Soma
`COUNT` Conta quantidade de linhas (conta nulos)
`MAX` Retorna maior valor
`MIN` Retorna menor valor

```sql
SELECT coluna, group_function(coluna)
FROM tabela
WHERE condicao
ORDER BY coluna1
```

#### Grupos de dados
Utilizar a cláusula `GROUP BY` para dividir os registros que serão agrupados em grupos de valores.
```sql
SELECT coluna, group_function(coluna)
FROM tabela
WHERE condicao
GROUP BY coluna1
ORDER BY coluna2

SELECT c_mktsegment
FROM customer
GROUP BY c_mktsegment;

-- Todas colunas no `SELECT`  devem ser usadas na cláusula do `GROUP BY`.
SELECT n.n_name, c.c_mktsegment
FROM customer c, nation n
GROUP BY c.c_mktsegment, n.n_name;
```

### Subconsultas
São úteis quando uma consulta baseia-se em valores desconhecidos.
```sql
SELECT coluna1, coluna2
FROM tabela
WHERE opr expre
	(SELECT coluna
	FROM tabela);
```
- Subconsultas são executadas primeiro.

*Ex*: Selecionar o preço total de uma encomenda, em que o preço total seja menor que o componente 776.
```sql
SELECT o_totalprice
FROM orders
WHERE o_totalprice < (
	SELECT p_retailprice
	FROM part
	WHERE p_partkey=776);
```

#### Subconsulta de várias linhas
`IN` Igual a qualquer membro da lista
`ANY` Compare com cada valor retornado
`ALL` Compare o valor com todos os valores retornados

```sql
SELECT o_totalprice
FROM orders
WHERE o_totalprice IN
	(SELECT p_retailprice
	FROM part
	WHERE p_type='LARGE POLISHED COPPER'
	GROUP BY p_retailprice);
	
SELECT o_totalprice
FROM orders
WHERE o_totalprice = ANY (
	SELECT MIN(l_extendedprice)
	FROM lineitem
	GROUP BY l_shipmode);
```

### Operadores de Conjunto
Combinar resultados de duas ou mais consultas em uma.

#### UNION
Combina todas as linhas de duas ou mais tabelas participantes da operação, **eliminando linhas duplicatas** resultantes.

*Ex*: Todas as contas dos clientes e todos os preços de varejo não duplicados.
```sql
SELECT c_acctbal
FROM customer
UNION
SELECT p_retailprice
FROM part
```

- `UNION ALL` faz o mesmo mas não se importa com duplicatas.

#### INTERSECT
Retorna todas as **linhas comuns** retornadas por duas ou mais consultas participantes.

*Ex*: A consulta exibe a conta do cliente que também é preço de varejo.
```sql
SELECT c_acctbal
FROM customer
INTERSECT
SELECT p_retailprice
FROM part;
```

#### EXCEPT
Retorna as linhas da primeira consulta que **não existem** na segunda consulta.

*Ex*: A consulta exibirá a conta do cliente que não é preço de varejo.
```sql
SELECT c_acctbal as "UNION"
FROM customer
EXCEPT 
SELECT p_retailprice
FROM part;
```

---

# Dependências funcionais

Pense um banco de dados descrito por um único esquema de relação **universal** (mega-relação) `R = {A_1, A_2, ..., A_n}`, sendo `A_1..A_n` atributos do banco.

Uma **dependência funcional (DF)** indicada por `X -> Y`, entre dois conjuntos de atributos `X` e `Y` que são subconjuntos de `R`, especifica uma *restrição* sobre possíveis tuplas que podem formar um estado de relação `r` de `R`. Isso significa que os valores de `Y` de uma tupla `r` dependem dos valores do componente `X`.
Ou seja, se dois registros tem o mesmo valor em `X`, eles **devem** ter o mesmo valor em `Y`.

O conjunto de atributos de `X` é chamado de **lado esquerdo** da DF, e `Y` é chamado de **lado direito**.

Uma DF é uma propriedade **semântica** ou **significado dos atributos**. Em geral, o projetista do esquema especifica as dependências funcionais que são *semanticamente óbvias*, as outras DFs podem ser deduzidas.

Não é possível confirmar uma DF a menos que saibamos *todos os estados legais possíveis* de `X`, mas com apenas *um único contraexemplo* é possível refutar uma DF.

Dados os conjuntos `X` e `Y`, e um atributo `A ∈ X`, seguem algumas definições:
- `X -> Y` é DF parcial se `(X-{A}) -> Y`.
- `X -> Y` é uma DF trivial se `Y⊆X`.
- `X -> Y` é uma DF transitiva se existe `X -> Z` e `Z -> Y`, e `Z` não é parte da chave primária.

## Normalização



