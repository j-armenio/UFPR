#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "libutil.h"
#include "libmember.h"
#include "liboptions.h"

/* Recebe uma string com o caminho de um arquivo e retorna esse caminho em formato
   de caminho relativo. */
char *getRelativePath(char *path)
{
    const char *prefix = "./";
    int prefixSize = strlen(prefix);

    /* Caso 1: "./file" */
    if (strncmp(path, prefix, prefixSize) == 0)
        return path;

    /* Caso 2: "/file" */
    if (path[0] == '/'){
        int pathSize = strlen(path);
        char *relativePath = (char *)malloc(pathSize + 2);
        relativePath[0] = '.';

        strncpy(relativePath + 1, path, pathSize);
        relativePath[pathSize + 1] = '\0';
        printf("relativePath1: %s\n", relativePath);

        return relativePath;
    }
    
    /* Caso 3: "file" */
    int pathSize = strlen(path);
    int relativePathSize = prefixSize + pathSize;

    char *relativePath = (char *)malloc(relativePathSize + 1);

    strncpy(relativePath, prefix, prefixSize);

    strncpy(relativePath + prefixSize, path, pathSize);
    relativePath[relativePathSize] = '\0';

    return relativePath;
}

/* Verifica se um arquivo já existe ou não, caso exista retorna 1. */
int checkIfFileExists(char *path)
{
    FILE *file = fopen(path, "rb");
    if (file != NULL){
        fclose(file);
        return 1;
    }
    return 0;
}

char *getOctalMode(int permissions) 
{
    static char perm[10];
    strcpy(perm, "---------");

    // Definir as permissões para dono, grupo e outros
    perm[0] = (permissions & 0400) ? 'r' : '-';
    perm[1] = (permissions & 0200) ? 'w' : '-';
    perm[2] = (permissions & 0100) ? 'x' : '-';
    perm[3] = (permissions & 040) ? 'r' : '-';
    perm[4] = (permissions & 020) ? 'w' : '-';
    perm[5] = (permissions & 010) ? 'x' : '-';
    perm[6] = (permissions & 04) ? 'r' : '-';
    perm[7] = (permissions & 02) ? 'w' : '-';
    perm[8] = (permissions & 01) ? 'x' : '-';

    return perm;
}

void removeExtension(char *filename) 
{
    int len = strlen(filename);
    int dotIndex = -1;

    // Procurar o último ponto '.' na string
    for (int i = len - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            dotIndex = i;
            break;
        }
    }

    // Se encontrou o ponto, truncar a string a partir dele
    if (dotIndex != -1) {
        filename[dotIndex] = '\0';
    }
}

void createDirectories(char* caminho) 
{
    char *copiaCaminho = strdup(caminho);
    char *savePtr;
    char *token = strtok_r(copiaCaminho, "/", &savePtr);
    char *caminhoAtual = NULL;

    while (token != NULL) {        
        if (caminhoAtual == NULL) {
            caminhoAtual = strdup(token);
        } else {
            size_t tamanho = strlen(caminhoAtual) + strlen(token) + 2;
            caminhoAtual = realloc(caminhoAtual, tamanho);
            strcat(caminhoAtual, "/");
            strcat(caminhoAtual, token);
        }
        struct stat st;
        if (stat(caminhoAtual, &st) == -1) {
            if (mkdir(caminhoAtual, 0700) != 0) {
                printf("Erro ao criar a pasta %s\n", caminhoAtual);
                return;
            }
        }
        
        token = strtok_r(NULL, "/", &savePtr);
        if (strlen(savePtr) == 0) {
            break;
        }
    }
    
    FILE* arquivo = fopen(caminho, "a+");
    if (arquivo != NULL) {
        // printf("Arquivo %s criado com sucesso.\n", caminho);
        fclose(arquivo);
    } else {
        printf("Erro ao criar o arquivo %s\n", caminho);
    }
    
    free(caminhoAtual);
    free(copiaCaminho);
}

// Copia o conteúdo binário de um arquivo para outro usando um buffer de 1024 bytes
void copyBinary(FILE *source, int fileSize, FILE *target)
{
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int iterations = fileSize / 1024;
    int rest = fileSize % 1024;
    int i;
    for (i = 0; i < iterations; i++) {
        fread(buffer, sizeof(char), 1024, source);
        fwrite(buffer, sizeof(char), 1024, target);
    }
    fread(buffer, sizeof(char), rest, source);
    fwrite(buffer, sizeof(char), rest, target);

    free(buffer);
}

void pasteMember(FILE *bkp, int copyStart, int size, int writeStart)
{
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int bCopy = copyStart;
    int bPaste = writeStart;

    int iterations = (size / 1024);
    int rest = size % 1024;

    for (int i = 0; i < iterations; i++) {
        fseek(bkp, bCopy, SEEK_SET);
        fread(buffer, 1, 1024, bkp);
        bCopy += 1024;
        fseek(bkp, bPaste, SEEK_SET);
        fwrite(buffer, 1, 1024, bkp);
        bPaste += 1024;
    }
    fseek(bkp, bCopy, SEEK_SET);
    fread(buffer, 1, rest, bkp);
    fseek(bkp, bPaste, SEEK_SET);
    fwrite(buffer, 1, rest, bkp);

    free(buffer);
}

void shiftRight(FILE *file, int bStart, int bEnd, int writeEnd)
{
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int blockSize = bEnd - bStart;
    int iterations = blockSize / 1024;
    int rest = blockSize % 1024;

    for (int i = 0; i < iterations; i++) {
        fseek(file, bEnd - 1024, SEEK_SET);
        fread(buffer, 1, 1024, file);
        bEnd -= 1024;
        fseek(file, writeEnd - 1024, SEEK_SET);
        fwrite(buffer, 1, 1024, file);
        writeEnd -= 1024;
    }
    fseek(file, bEnd - rest, SEEK_SET);
    fread(buffer, 1, rest, file);
    fseek(file, writeEnd - rest, SEEK_SET);
    fwrite(buffer, 1, rest, file);

    free(buffer);
}

void shiftLeft(FILE *file, int bStart, int bEnd, int writeStart)
{
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    if (buffer == NULL) {
        printf("Erro ao alocar memória para o buffer.\n");
        return;
    }

    int blockSize = bEnd - bStart;
    int iterations = blockSize / 1024;
    int rest = blockSize % 1024;

    for (int i = 0; i < iterations; i++) {
        fseek(file, bStart, SEEK_SET);
        fread(buffer, 1, 1024, file);
        bStart += 1024;
        fseek(file, writeStart, SEEK_SET);
        fwrite(buffer, 1, 1024, file);
        writeStart += 1024;
    }
    fseek(file, bStart, SEEK_SET);
    fread(buffer, 1, rest, file);
    fseek(file, writeStart, SEEK_SET);
    fwrite(buffer, 1, rest, file);

    free(buffer);
}