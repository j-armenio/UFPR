#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *filePath = argv[1];
    FILE *file = NULL;
    int position, prevPosition;
    char letter;
    char *poema;


    file = fopen(filePath, "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }
 
    poema = malloc(2 * sizeof(char));
    if (poema == NULL)
    {
        printf("Erro ao alocar string poema\n");
        exit(1);
    }

    int maiorPos = 2;

    fread (&position, 4, 1, file);
    fread (&letter, 1, 1, file);
    while (! feof(file)){
        if (position >= maiorPos){
            poema = realloc(poema, (position + 1) * sizeof(char));
            if (poema == NULL)
            {
                printf("Erro ao reallcar string poema\n");
                exit(1);
            }
            poema[position] = '\0';
            maiorPos = position;
        }
        poema[position] = letter;
        prevPosition = position;
        fread (&position, 4, 1, file);
        fread (&letter, 1, 1, file);
    }

    printf("%s\n", poema);

    /* ----------------------------- */

/*
    int strSize = 0;
    while (fread(&position, 4, 1, file) && fread(&letter, 1, 1, file))
        strSize++;

    poema = malloc(sizeof(char) * strSize);
    if (poema == NULL)
    {
        printf("Erro ao alocar string poema\n");
        exit(1);
    }

    rewind(file);
    // fseek(file, 0, SEEK_SET);

    while (fread(&position, 4, 1, file) && fread(&letter, 1, 1, file))
        poema[position] = letter;

    printf("%s\n", poema);
*/

    fclose(file);
}