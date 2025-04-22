# LLVM

## Identificadores

São formados pelo padrão: `[%@][-a-zA-Z$._][-a-zA-Z$._0-9]*`

**@** identificador global
**%** identificador local

### Variáveis globais

São alocadas em memória, e devem ser inicializadas. Basicamente um ponteiro para uma região da memória.

Ex: variável global do tipo inteiro 32 bits com valor inicializado 2
`@x = global i32 2`

## Tipos

LLVM-IR é fortemente tipada. 
O formato para inteiros é **i\<quantidade de bits>** como i32, i1 (usada em comparações, como se fosse um tipo booleano).
Ponto flutuante é definido como **float** e **double**, e as instruções para esse tipo são diferentes das sobre inteiros.
**void** pode ser usado para definir uma função que não retorna valor algum.

## Estrutura

Programas LLVM são compostos de módulos, onde cada módulo contém funções e variáveis globais. Cada função será composta de um ou mais blocos basicos, e cada bloco contém instruções.

#### Bloco Básico

Inicia com um label, ou o recebe de forma implicita. Contém um conjunto de instruções e finaliza com uma instrução terminadora.
Tem entrada e saída (inst. terminadora), portando não tem instruções de desvio "no meio" dos blocos.

Ex: Possui três blocos básicos. O primeiro começa com o rótulo de entrada *Test* e termina com uma instrução de desvio *br*. O segundo inicia com rótulo *IfEqual* e termina com um retorno. O terceiro inicia com *IfUnequal* e termina com retorno. O trecho implementa uma comparação de igualdade entre os regitradores **%a** e **%b**, caso sejam iguais, o bloco *IfEqual* será executado e retorna 1, caso contrário inicia *IfUnequal* e retorna zero.

```llvm
Test:
    %cond = icmp eq i32 %a, %b
    br i1 %cond, label %IfEqual , label %IfUnequal
IfEqual:
    ret i32 1
IfUnequal:
    ret i32 0
```

## Funções

São criadas com a palavra-chafe *define*. Podem possuir um tipo de retorno e parâmetros de entrada. Parâmetros de entrada podem ser variáveis simples ou ponteiros.

Ex: Recebe dois parâmetros de entrada do tipo inteiro, e retorna a soma dos dois. Como funções tem escopo global, é identificado por @.

```llvm
define i32 @soma(i32 %x, i32 %y) {
    %1 = add i32 %x, %y
    ret i32 %1
}
```

Para executar a função usamos a instrução *call*.

Ex2: Utiliza a função soma e também imprime um inteiro na tela. As variáveis %1 e %2 são inteiros inicializados, respectivamente, com valores 1 e 2. A função vai receber esses dois valores por cópia e retornar a soma deles. Por fim, esse resultado é impresso na tela com uma quebra de linha.

```llvm
declare i32 @printf(ptr noundef, ...)
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @soma(i32 %x, i32 %y) {
    %1 = add i32 %x, %y
    ret i32 %1
}

define i32 @main() {
    %1 = add i32 0, 1
    %2 = add i32 0, 2
    %3 = call i32 @soma(i32 %1, i32 %2)
    %4 = call i32 (ptr, ...) @printf(ptr noundef @write_int, i32 %3)
    ret i32 0
}
```

## Static Single Assignment (SSA)

LLVM-IR é uma linguagem SSA, o que significa que todo registrador em uma função possui apenas uma única atribuição (single assignment). Isso permite muitas técnicas de otimização.

## Memória: alloca, store e load

O gerênciamento de memória é feito de forma automática, sem que o usuário se preocupe na forma que a pilha, ou alguma estrutura, seja organizada.

*alloca* vai retornar um ponteiro para uma região da memória que será automaticamente desalocada ao final da função. Em conjunto usaremos *store* para armazenar e *load* para fazer a leitura dessa região.

Ex: aloca um espaço na memória, armazena o valor 15 e faz leitura para uma nova variável. Depois, esse valor é incrementado e salvo novamente na posição de memória. 

```llvm
%x = alloca i32
store i32 15, ptr %x
%1 = load i32, ptr %x
%2 = add i32 %1, 1
store i32 %2, ptr %x
```

## Condicional e Desvio

Para executar condicionais, usamos as instruções *icmp* para inteiros e *fcmp* para ponto-flutuante. *icmp* permite usar as comparações:
**eq**: igual; **ne**: não é igual; **ugt**: maior que, sem sinal; **uge**: maior ou igual, sem sinal; **ult**: menor que, sem sinal; **ule**: menor ou igual, sem sinal; **sgt**: maior que, com sinal; **sge**: maior ou igual, com sinal; **slt**: menor que, com sinal; **sle**: menor ou igual, com sinal

*fcmp* é similar, mas em vez de considerar sinal ('u' e 's'), ela usa ordered e unordered ('o' e 'u').
Essas instruções de comparação retornam um valor do tipo *i1*, verdadeiro(1) e falso(0).

Ex: if simples que verifica se dois valores são iguais.

```llvm
Test:
    %cond = icmp eq i32 %a, %b
    br i1 %cond, label %IfEqual, label %IfUnequal
IfEqual:
    ret i32 1
IfUnequal
    ret i32 0
```

## Exemplo: Fatorial recursivo

```pascal
function fat(n: integer): integer;
begin
    if n <= 1
        fat := 1
    fat := n * fat(n - 1);
end.
```

```llvm
declare i32 @printf(ptr noundef, ...)
declare i32 @__isoc99_scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @fat(i32 %x) {
    %1 = icmp sle i32 %x, 0
    br i1 %1, label %retFim, labal %retRec
retFim:
    ret i32 1
retRec:
    %2 = sub i32 %x, 1
    %3 = call i32 @fat(i32, %2)
    %4 = mul i32 %x, %3
    ret i32 %4
}

define i32 @main() {
    %1 = alloca i32
    %2 = call i32 (ptr, ...) @__isoc99_scanf(ptr @read_int, ptr %1)
    %3 = load i32, ptr %1
    %4 = call i32 @fat(i32 %3)
    %5 = call i32 (ptr, ...) @printf(ptr noundef @write_int, i32 %4)
    ret i32 0
}
```
