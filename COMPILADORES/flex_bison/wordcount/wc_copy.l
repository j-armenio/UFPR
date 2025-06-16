%{
/*  Cabeçalho
    Esse código é copiado diretamente para o arquivo .c gerado pelo Flex. */
int caracteres = 0; /* número total de caracteres */
int palavras = 0;   /* número de palavras */
int linhas = 0;     /* número de quebras de linhas */
%}

/*  Regras léxicas */
/*  Declaração de expre4ssão regular com nome, usadas nas regras. */
letra [a-zA-Z] /* Representa qualquer letra do alfabeto, minuscula e maiscula */

%%
/* Regras que definem o que fazer quando encontrar certos padrões de texto. */

{letra}+ { palavras++; caracteres += strlen(yytext); } /* uma ou mais letras, representa uma palavra -> aumenta contador palavras e soma caracteres da palvra ao total */
\n { caracteres++; linhas++; }                         /* quebra de linha -> aumenta contador de linhas e caracteres */
. { caracteres++; }                                    /* qualquer outro caractere individual (espaço, vírgula, etc) -> aumenta caracteres */

%%

int main(int argc, char **argv) {
    yylex(); /* gerada automaticamente pelo Flex e começa a analisar a entrada padrão (stdin) */
    printf("linhas: %d\npalavras: %d\ncaracteres: %d\n", linhas, palavras, caracteres);
    return 0;
}
