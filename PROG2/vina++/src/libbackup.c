#include <stdio.h>
#include <stdlib.h>

#include "libbackup.h"

// Le o content do file e insere esse content no backup
void FileContentToBackup(FILE *backupFile, char *targetFilePath)
{
    // Abre o arquivo que quer copiar
    FILE *targetFile = fopen(targetFilePath, "rb");
    if (! targetFile){
        printf("Erro ao abrir arquivo %s.\n", targetFilePath);
        exit(1);
    }

    // Copiar e colar o conteudo do arquivo para o backup usando fread e fwrite
    char *buffer;
    size_t fileSize;

    // Obtem o tamanho do arquivo
    fseek(targetFile, 0, SEEK_END);
    fileSize = ftell(targetFile);
    rewind(targetFile);

    // Aloca memória para o buffer
    buffer = (char *)malloc(sizeof(char) * fileSize);
    if (! buffer){
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // Copia o conteudo do arquivo para o buffer e escreve no backup
    fread(buffer, sizeof(char), fileSize, targetFile);
    fwrite(buffer, sizeof(char), fileSize, backupFile);

    free(buffer);
    fclose(targetFile);
}

