#include <stdio.h>
#include <stdlib.h>
 
#define LINESIZE 1024
 
int conta_caracteres(FILE *arq){
    int cont = 0;
    char c;

    while (c != EOF){
        c = fgetc(arq);
        cont++;
    }
    return cont-1;
}

int media_arq(FILE *arq){ /* nao funciona */

}

int main ()
{
    FILE *arq;
    int linhas, soma;
    char line[LINESIZE-1];

    arq = fopen("test.txt", "r");
    if (!arq){
        perror("Falha ao abrir test.txt");
        exit(1);
    }

    fgets(line, LINESIZE, arq);
    linhas = 1;

    while (!feof(arq)){
        fgets(line, LINESIZE, arq);
        
        
        linhas++;
    }

    printf("%d\n", soma);

    fclose(arq);
}