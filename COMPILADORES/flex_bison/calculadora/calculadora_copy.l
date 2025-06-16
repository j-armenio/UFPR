/* diz ao Flex para acompanhar o número da linha, útil para erros */
%option yylineno

%{
#include "calculadora.tab.h" // Inclui o header gerado pelo Bison -> isso define tokens
char err_buffer[500]; 
int yyerror(char *s); // declara função de erro
%}

NUM [0-9]+ // define padrão NUM como sequência de dígitos (número inteiro)

%%

{NUM} {yylval = atof(yytext); return NUMERO;}
/*  yytext contém o texto reconhecido
    yylval é usado para passar valores do Flex para o Bison */
"+" {return MAIS;}
"-" {return MENOS;}
"*" {return MUL;}
"/" {return DIV;}
"(" {return AP;}
")" {return FP;}
// Cada operador ou parêntese é transformado em token que será interpretado no Bison
\n { return EOL;}
[ \t] {/*ignorar espacos em branco, incluindo tabs*/}
. { sprintf(err_buffer, "caracter estranho '%c'", *yytext); yyerror(err_buffer); }

%%

// Sem codigo aqui, flex sera usado pelo bison e nao 'standalone'
