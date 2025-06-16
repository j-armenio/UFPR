/* Responsável por ler os caracteres do código-fonte e transformá-los em tokens para o Bison
Um arquivo .l é dividido em três partes

P1: CÓDIGO AUXILIAR (opccional) %{ ... %}
%%
P2: EXPRESSÕES REGULARES + AÇÕES regex { ação }
%%
P3: CÓDIGO C (opcional)
*/

// Diz ao Flex para manter o número da linha atualizado automaticamente na variável yylineno
%option yylineno

%{ 
#include "tabela_simbolos.h"
#include "compilador.h"
#include "compilador.tab.h"
%}


id [a-zA-Z][a-zA-Z1-9]*

digitos [0-9]+

fracao_opcional (.{digitos})?

expoente_opcional ([Ee][+-]?{digitos})?
%%
// Regras léxicas


"int" { return INT;}

"float" { return FLOAT;}

"void" { return VOID;}

"return" { return RETURN;}

"+" |
"-" |
"*" |
"/" |
";" |
"," |
"(" |
")" |
"{" |
"}" |
"=" { return yytext[0];}



// Reconhece números inteiros ou reais e retorna token NUM
{digitos}{fracao_opcional}{expoente_opcional} {
    yylval.lexema = strdup(yytext);
    return NUM;
}

// Reconhece qualquer identificador (como x, soma, area1) e retorna token ID
{id} {
    yylval.lexema = strdup(yytext);
    return ID;
}

\n {
    //return EOL;
}

[ \t] {/*ignorar espacos em branco, incluindo tabs*/}

%%

// Sem codigo aqui, flex sera usado pelo bison e nao 'standalone'
