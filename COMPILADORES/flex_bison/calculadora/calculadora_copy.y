%{
#include <stdio.h>
int yylex();
int yyerror(char *s);
%}

// Define os tokens que virão do Flex
%token NUMERO MAIS MENOS MUL DIV AP FP EOL

// Precedência e associatividade
// Diz que + e - tem mesma precedência e associatividade à esquerda
%left MAIS MENOS
// também mas tem precedência maior porque vem depois
%left MUL DIV

%%

// $1 se refere ao primeiro simbulo da regra $2 ao segundo e assim por diante

CALC: /* empty */
 | CALC EXP EOL { printf("%d\n> ", $2); }
 | CALC EOL { printf("> "); } // linha em branco
 ;

EXP: FATOR // EXP representa adições/subtrações
 | EXP MAIS EXP { $$ = $1 + $3; } // $$ é o valor da regra inteira, é tipo o return dela
 | EXP MENOS FATOR { $$ = $1 - $3; }
 ;

FATOR: TERMO // multiplicação e divisão
 | FATOR MUL TERMO { $$ = $1 * $3; }
 | FATOR DIV TERMO { $$ = $1 / $3; }
 ;

TERMO: NUMERO // números ou expressões entre parênteses
 | AP EXP FP { $$ = $2; }
 ;


%%

int main(int argc, char ** argv) {
  printf("> ");
  yyparse(); // inicia o parser que chama o yylex() do flex
  return 0;
}

int yyerror(char *s) {
  fprintf(stderr, "Erro: %s\n", s);
  return 0;
}

