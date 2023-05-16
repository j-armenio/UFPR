#include <stdio.h>
#include <stdlib.h>

#include "libdirectory.h"
#include "libutil.h"

/* Retorna 1 caso o caminho do arquivo recebido seja absoluto e 0 caso seja relativo */
int isAbsolutePath(char* path)
{
    if (path[0] == '/' || path[0] == '.')
        return 0;
    else
        return 1;
}