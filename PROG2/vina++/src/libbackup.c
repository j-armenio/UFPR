#include <stdio.h>
#include <stdlib.h>

#include "libbackup.h"

// Le o content do file e insere esse content no backup
void FileContentToBackup(FILE *backupFile, char *targetFilePath)
{
    FILE *file = NULL;
    char *buffer;
    unsigned int fileSize = 0;

    file = fopen(targetFilePath, "rb");
    if (! file){
        printf("Erro ao abrir arquivo %s\n", targetFilePath);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = (char *)malloc(sizeof(char) * fileSize);
    if (! buffer){
        printf("Erro ao alocar buffer.\n");
        exit(1);
    }
    fread(buffer, fileSize, 1, file);
    fwrite(buffer, fileSize, 1, backupFile);

    free(buffer);
    fclose(file);
}

