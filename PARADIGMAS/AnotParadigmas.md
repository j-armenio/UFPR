# Paradigmas de Programação

## Sumário

1. [Introdução a Linguagem Java](#introdução-a-linguagem-java)
2. [Linguagem e Paradigma de Programação](#linguagem-e-paradigma-de-programação)
3. [Encapsulamento](#encapsulamento)

## Introdução a Linguagem Java

* Orientada a objetos
* Gerenciamento automático de memória
* Portabilidade
* Composto por dois ambientes:
    * JDK (Java Development Kit) - Ambiente de Desenvolvimento:
    * JRE (Java Runtime Environment) - Ambiente de Execução: Consiste na JVM e na Java API
        * javac - compilador
        * jar - empacotador
        * javadoc - ferramenta para geração de documentação
        * JVM (JAVA Virtual Machine): Máquina virtual que interpreta e executa o código java compilado.

O código fonte é o **BemVindo.java** e após a compilação usando ```javac``` irá gerar o cexecutavel **BemVindo.class**, e para executar use ```java BemVindo```.

### Classes

Todo programa consiste de pelo menos uma declaração de classe.

```java
public class BemVindo {
    ...
}
```
A palavra class introduz uma classe.

Regras e conveções:  
* Primeiro caractere: letra, underscore, cifrão  
* Não é permitido espaço em bracos e operadores  
* Não é permitido palavras reservadas  
* Iniciam com letra maiúscula e usam CamelCase (ex: **SampleClassName**)

Programa Principal: Método main()

```java
public class BemVindo {
    public static void main(String[] args) {
        // Método principal
        System.out.println("Olá, seja bem-vindo!");
    }
}
```

### Entrada e Saída

```java
import java.util.Scanner;

public class Principal {
    public static void main(String[] args)
    {
        Scanner input = new Scanner(System.in); // Faz a instância do Scanner
        int num;
        System.out.print("Digite um número: ");
        
        num = input.nextInt;

        System.out.println("Número digitado: " + num);

        input.close();
    }
}
```

### Tipos de Dados
* lógico: boolean
* textual: char e String
* ponto flutuante: float ou double
* inteiro: byte (8 bits), short (16 bits), int (32 bits) e long (64 bits)

Váriavel final: váriavel não modificável, constante.  
```final String msg = "Bem vindo!";```

## Linguagem e Paradigma de Programação

Uma linguagem é composta por dois conjuntos de regras para definir programas:  
ex:
```java
if (x > 0)
    x = x + 1
```  

* Sintaxe: representação simbólica; ```if (<expr>) <instrução>```
* Semântica: conceito que ela representa; se o valor da expressão for verdadeiro, a variável x é incrementada em 1

### Tipos de Paradigmas

#### Imperativo
Instruções passadas devem especificar o passo a passo das ações que serão executadas. Fortran, Pascal, Cobol, C

#### Estruturado
Também conhecido como procedural, capacidade de modularização (métodos, funções, rotinas). As instruções podem ser formadas por umas das seguintes estruturas: sequência, condicionais, repetição.

#### Orientado a Objetos
Organização do código por objetos que contém dados, estados próprios e métodos que alteram e recuperam os dados/estados. Os objetos comunicam entre si para compor a lógica do programa. Java, C++, Simula, Smaltalk, C#

#### Declarativo
Fluxo lógico implícito. Linguagens de alto nível que permite ao programador dizer apenas o que deseja. SQL

#### Lógico
Especifica-se apenas fatos e regras de inferência. Retorno escrito em forma de pergunta.

#### Funcional 
Descreve a computação como uma expressão a ser avaliada. A principal forma de estruturar o programa é pela definição e aplicação de funções.

> Na prática, muitas linguagem são ***multiparadigmas***.

## Encapsulamento







