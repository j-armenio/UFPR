#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIN_MATRIZ 500
#define LINESIZE 1024

char *file_to_buffer(char *file_name)
{
    FILE *file;
    char *buffer;
    int tam;
    
    file = fopen(file_name, "r");
    if (! file){
        printf("Failed to open %s\n", file_name);
        exit(1);
    }

    if (file){
        fseek(file, 0, SEEK_END);
        tam = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc ((sizeof(char) * tam) + 1);
        if (buffer)
            fread(buffer, 1, tam, file);
    }
    fclose(file);
    return buffer;
}

char **aloca_matriz(int lin, int col)
{
    /* 
        linhas = quantidade de ponteiros
        colunas = tamanho da string 
    */
    char **mat;
    int i;

    mat = malloc(sizeof(char*) * lin);
    for (i = 0; i < lin; i++)
        mat[i] = malloc(sizeof(char) * col);
    return mat;
}

void desaloca_matriz(char **mat, int lin)
{
    int i;
    for (i = 0; i < lin; i++)
        free(mat[i]);
    free(mat);
}

char **separa_tag_para_matriz(char *buffer_original, char *tag, char *prop)
{
    char **matriz = aloca_matriz(LIN_MATRIZ, LINESIZE);
    char *buffer = buffer_original;
    char *prop_ptr;
    int i;

    while ((buffer = strstr(buffer, tag)) != NULL){ // vai ate a tag
        // vai ate a prop mais proxima da tag
        prop_ptr = strstr(buffer, prop);
        if (! prop_ptr){
            printf("Propriedade não encontrada\n");
            exit(1);
        }

        // pula o "nome" da tag e remove o "=
        prop_ptr = (prop_ptr + strlen(prop)) + 2;

        while (*prop_ptr != ""){ // vai lendo ate achar ""
            matriz[i] = prop_ptr;
            prop_ptr++;
            i++;
        }
        matriz[i] = '\0';

        // pula a tag no buffer
        buffer = prop_ptr;
    }

    return matriz;
}

/*  gcc -Wall teste.c -o teste  */

int main()
{
    char file_name[100]; 
    char *buffer;
    char **periodicos;

    scanf("%s", file_name);

    buffer = file_to_buffer(file_name);

    // OBJETIVO: busca as conferencias no xml -> copia e coloca elas na matriz

    // 1. busca as conferencias no xml
    
    char *conf_tag = "DETALHAMENTO-DO-TRABALHO";
    char *conf_prop = "TITULO-DO-PERIODICO-OU-REVISTA";
    periodicos = separa_tag_para_matriz(buffer, conf_tag, conf_prop);


    // Libera memoria
    desaloca_matriz(periodicos, LIN_MATRIZ);
    free(buffer);
}