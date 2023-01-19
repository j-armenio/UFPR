#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 1024
#define TAM 1024

/* Ex 1 */
void numero_de_caracteres_txt(){
    FILE *arq;
    char txt_title[TAM];
    char c;
    int cont;

    scanf("%s", txt_title);
    
    /* abre o arquivo para leitura */
    arq = fopen("poema1.txt", "r");
    if (! arq){
        printf("Erro ao abrir %s\n", txt_title);
        exit(1);
    }

    /* le os caracteres ate o fim do arquivo */
    c = fgetc (arq);
    while (c != EOF){
        cont++;
        c = fgetc(arq);
    }

    fclose(arq);

    printf("%d\n", cont);
}

int main(){
    numero_de_caracteres_txt();
}