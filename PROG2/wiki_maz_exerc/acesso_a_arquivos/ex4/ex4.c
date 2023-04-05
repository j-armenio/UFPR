#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 1024

int main()
{
    FILE *file;
    char **mat;
    char c;
    int size_1, size_2, i, j;
    
    file = fopen("mapa.txt", "r");
    if (! file){
        printf("Erro ao abrir file.\n");
        return 1;
    }

    size_1 = 0;
    size_2 = 0;
    i = 1;

    c = fgetc(file);
    while (c != ' ')
    {   /* ARITMETICA ERRADA */
        size_1 = (size_1 * i) + c;
        i = i * 10;
        c = fgetc(file);
    }

    i = 1;

    c = fgetc(file);
    while (c != '\n')
    {   /* ARITMETICA ERRADA */
        size_2 = (size_2 * i) + c;
        i = i * 10;
        c = fgetc(file);
    }

    printf("%d\n", size_1);
    printf("%d\n", size_2);

    for (i = 0; i <= size_1; i++){
        mat[i] = malloc(sizeof(char));
        for (j = 0; j <= size_2; j++){
            mat[i][j] = malloc(sizeof(char));
        }
    }

    i = 0;
    j = 0;
    while (c != EOF)
    {
        j++;
        if (j <= size_2){
            j = 0;
            i++;
        }
        c = fgetc(file);
    }


    for (i = 0; i <= size_1; i++){
        free(mat[i]);
        for (j = 0; j <= size_2; j++){
            free(mat[i][j]);
        }
    }
    fclose(file);
}