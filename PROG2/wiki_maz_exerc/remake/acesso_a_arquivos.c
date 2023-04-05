#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 1024

/* Ex 1 */
void numero_de_caracteres_txt(){
    FILE *arq;
    int count = 0;
    char c = 0;
    char string[LINESIZE+1];

    scanf("%s", string);

    /* abre o arquivo para leitura */
    arq = fopen(string, "r");
    if (!arq){
        printf("Erro ao abrir arquivo.\n");
        exit(1);
    }

    /* le caracter do arquivo */
    c = fgetc(arq); /* le um caractere */
    
    while (c != EOF){ /* enquanto eh diferente do fim do arquivo */
        count++;
        c = fgetc(arq);
    }

    fclose(arq);

    printf("%d\n", count);
}

void numero_de_caracteres_2(){
    FILE *arq;
    char string[LINESIZE+1];

    scanf("%s", string);

    arq = fopen(string, "r");
    if (!arq){
        perror("Erro ao abrir arquivo.\n");
        exit(1);
    }

    fseek(arq, 0, SEEK_END);
    printf("%ld\n", ftell(arq));
    fclose(arq);
}

/* Ex 2 */
void le_e_faz_media(){
    FILE *arq;

    scanf("%s", string);

}

int main(){

}