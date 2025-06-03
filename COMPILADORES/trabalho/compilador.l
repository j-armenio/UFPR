/* FLEX analisador léxico/lexer 
Responsável por ler o código-fonte caractere por caractere e reconhece padrões
  (palavras, números, símbolos) usando expressões regulares. Cada padrão é
  associado a um token, que será enviado ao Bison.

Um arquivo .l é dividido em três partes

declarações
%%
regras de tradução: Padrão { Ação }
%%
funções auxiliares
*/

%option yylineno

%{
#include "tabela_simbolos.h"
#include "compilador.h"
#include "compilador.tab.h"
%}


ID [a-zA-Z_][a-zA-Z1-9_]*

NUM [0-9]+(\.[0-9]+)?([Ee][+-]?[0-9]+)?


%%


"program"      { return PROGRAM; }
"var"          { return VAR; }
"integer"      { return TK_INTEIRO; }
"real"         { return TK_REAL; }
"function"     { return FUNCTION; }
"procedure"    { return PROCEDURE; }
"begin"        { return BEGIN_TOKEN; }
"end"          { return END; }
"if"           { return IF; }
"then"         { return THEN; }
"else"         { return ELSE; }
"while"        { return WHILE; }
"do"           { return DO; }
":="           { return OPERADOR_ATRIBUICAO; }
"="            { yylval.lexema = strdup(yytext); return OPERADOR_RELACIONAL; }
"<>"           { yylval.lexema = strdup(yytext); return OPERADOR_RELACIONAL; }
">"            { yylval.lexema = strdup(yytext); return OPERADOR_RELACIONAL; }
">="           { yylval.lexema = strdup(yytext); return OPERADOR_RELACIONAL; }
"<"            { yylval.lexema = strdup(yytext); return OPERADOR_RELACIONAL; }
"<="           { yylval.lexema = strdup(yytext); return OPERADOR_RELACIONAL; }
"+"            { return MAIS; }
"-"            { return MENOS; }
"or"           { return OR; }
"*"            { yylval.lexema = strdup(yytext); return OPERADOR_MULTIPLICATIVO; }
"/"            { yylval.lexema = strdup(yytext); return OPERADOR_MULTIPLICATIVO; }
"div"          { yylval.lexema = strdup(yytext); return OPERADOR_MULTIPLICATIVO; }
"mod"          { yylval.lexema = strdup(yytext); return OPERADOR_MULTIPLICATIVO; }
"and"          { yylval.lexema = strdup(yytext); return OPERADOR_MULTIPLICATIVO; }
"."            { return PONTO_FINAL; }
";"            { return PONTO_VIRGULA; }
","            { return VIRGULA; }
":"            { return DOIS_PONTOS; }
"("            { return ABRE_PARENTESES; }
")"            { return FECHA_PARENTESES; }

{ID} {
    yylval.lexema = strdup(yytext);
    return ID;
}

{NUM} {
    yylval.lexema = strdup(yytext);
    return NUM;
}

[ \t\r]+ ; // ignora espaços e tabulações

\n { /* empty */ }

. {
    printf("Caractere não reconhecido: %s\n", yytext);
    return -1;
}


%%
