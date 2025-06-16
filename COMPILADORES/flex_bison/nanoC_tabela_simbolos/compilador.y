/* Usa tokens gerados pelo .l para verificar se o código-fonte segue a gramática da linguagem.
Ele também constroi e gerencia a tabela de símbolos, além de preparar o caminho para
  a geração de código (LLVM-IR).

Estrutura geral:
P1: Código C e declarações (%{ ... %}, %token, %union etc.)
%%
P2: Gramática + ações em C
%%
P3: Código C auxiliar (main, yyerror, etc.)
*/
%{
#include <stdio.h>
#include <stdlib.h>
#include "tabela_simbolos.h"
#include "compilador.h"

int yylex();
extern FILE *yyin;
extern int yylineno;
FILE *log_file, *out_file;

struct tabela_simbolos * tab_simbolos = NULL; // é a tabela de símbolos global
int escopo_atual = 0; // rastreia o nível de escopo
%}

// Mostra errors de parsing com mais detalhes
%define parse.error detailed

// Isso define os tipos possíveis de yylval
%union { // apenas um dos tipo, lexema ou lista_s é usado por vez
    char *lexema; // usado em ID e NUM
    struct lista_simbolo * lista_s; // para listas de variáveis e argumentos
    Tipo tipo; // guarda INT, REAL, etc
}


// Declaração dos tokens (vem do lexer)
%token INT FLOAT VOID RETURN
%token <lexema>NUM <lexema>ID 

// Declaração dos tipos de não-terminais
%type <lista_s>LISTA_IDS <lista_s>LISTA_ARGS
%type <tipo>TIPO 

// Diretivas que dizem ao bison:
// Associatividade dos operadores: %left
// Precedẽncia: quem vem depois tem maior precedência
%left '+' '-' 
%left '*' '/' 

%%
// Define a gramática da linguagem e o que fazer quando cada regra for reconhecida

// Regra principal do parser, que define o que um programa válido tem:
// 1. declarações de variáveis globais (LISTA_DECLARACOES)
// 2. declarações de funções (DECLARACOES_FUNCOES)
// depois de reconhecer tudo, imprime a tabela de símbolos final
PROGRAMA: LISTA_DECLARACOES
        DECLARACOES_FUNCOES { 
        imprime_tabela_simbolos(log_file, tab_simbolos);
        }
        ;

// Permite várias declarações de variáveis, ou nenhuma
LISTA_DECLARACOES: LISTA_DECLARACOES DECLARACAO
                 | /*empty*/
                 ;

// Reconhece uma declaração de variáveis e registra na tabela de símbolos
DECLARACAO: TIPO LISTA_IDS ';' {
    atualiza_tipo_simbolos($2,$1);
    tab_simbolos = insere_simbolos_ts(tab_simbolos, $2);
    imprime_tabela_simbolos(log_file, tab_simbolos);
    }
    ;

// Retorna o valor da enum Tipo
TIPO: INT {$$ = INTEIRO;}
    | FLOAT {$$ = REAL;}
    | VOID {$$ = VAZIO;}
    ;

// Constroi uma lista de variáveis ainda sem tipo
LISTA_IDS: 
    ID {
        $$ = insere_lista_simbolo(NULL, novo_simbolo3($1, VARIAVEL, escopo_atual));
        }
    | LISTA_IDS ',' ID {
        $$ = insere_lista_simbolo($1, novo_simbolo3($3, VARIAVEL, escopo_atual));
        }
    ;

// Permite múltiplas funções
DECLARACOES_FUNCOES: DECLARACOES_FUNCOES DECLARACAO_FUNCAO
                    | DECLARACAO_FUNCAO
                    ;

/*
Declara uma função com:
    tipo de retorno (TIPO)
    nome (ID)
    argumentos (LISTA_ARGS)
    corpo (LISTA_ENUNCIADOS)
*/
DECLARACAO_FUNCAO:
    TIPO ID {
        struct simbolo *func = novo_simbolo4($2, FUNCAO, escopo_atual, $1);
        tab_simbolos = insere_simbolo_ts(tab_simbolos, func);
        escopo_atual++;
    }
    '(' LISTA_ARGS ')' {
        struct simbolo *func = busca_simbolo(tab_simbolos, $2);
        insere_func_args(func, $5);
        tab_simbolos = insere_simbolos_ts(tab_simbolos, $5);
        fprintf(log_file, "Iniciando funcao %s\n",$2);
    }
    '{' LISTA_ENUNCIADOS '}' {
        imprime_tabela_simbolos(log_file, tab_simbolos); 
        tab_simbolos = remove_simbolos(tab_simbolos, escopo_atual); 
        escopo_atual--;
        fprintf(log_file, "Finalizando funcao %s\n",$2);
    }
    ;

// Cria uma lista de parâmetros para a função. Cada argumento:
// é um símbolo do tipo VARIAVEL
// tem um tipo e nome
// é armazenado em uma lista
LISTA_ARGS: TIPO ID {
                $$ = insere_lista_simbolo(NULL, novo_simbolo4($2, VARIAVEL, escopo_atual, $1));
            }
          | LISTA_ARGS ',' TIPO ID {
                $$ = insere_lista_simbolo($1, novo_simbolo4($4, VARIAVEL, escopo_atual, $3));
            }
          | /*empty*/ {
            $$ = NULL;
            }
          ;

// Permite vários comandos no corpo da função
LISTA_ENUNCIADOS : ENUNCIADO
                 | LISTA_ENUNCIADOS ENUNCIADO
                 ;

ENUNCIADO: ID '=' EXPRESSAO ';'
         | DECLARACAO 
         | RETURN EXPRESSAO ';'
                  
EXPRESSAO: EXPRESSAO '+' EXPRESSAO
         | EXPRESSAO '-' EXPRESSAO
         | EXPRESSAO '*' EXPRESSAO
         | EXPRESSAO '/' EXPRESSAO
         | FATOR
         ;
         
FATOR: ID
     | NUM
     | '(' EXPRESSAO ')'
     ;

%%

// Se um arquivo é passado por linha de comando, ele é aberto como entrada
int main(int argc, char ** argv) {
    log_file = fopen ("compilador.log", "w");
    //out_file = fopen ("output.ll", "w");
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        yylineno=1;
        yyparse(); // inicia o parser
    } else if (argc == 1) {
        yyparse();
    }    
    fprintf(log_file, "Finalizando compilacao\n");
    //fclose(out_file);
    fclose(log_file);
    return 0;
}

int yyerror(const char *s) {
  fprintf(stderr, "Erro na linha %d: %s\n", yylineno,s);
  exit(1);
  //return 0;
}

