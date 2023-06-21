#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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