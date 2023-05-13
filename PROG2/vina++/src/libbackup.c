#include <stdio.h>
#include <stdlib.h>

#include "libbackup.h"

// Le o content do file e insere esse content no backup
void contentFromFileToBackup(FILE *backupFile, char *targetFilePath)
{
    // Abre o arquivo que quer copiar
    FILE *targetFile = fopen(targetFilePath, "rb");
    if (! targetFile){
        printf("Erro ao abrir arquivo %s.\n", targetFilePath);
        exit(1);
    }

    // Copia o content do arquivo para o backup

    // Le char por char do arquivo
    int character;
    while ((character = fgetc(targetFile)) != EOF) {
        // Escreve o char no arquivo de backup
        fputc(character, backupFile);
    }

    fclose(targetFile);
}

