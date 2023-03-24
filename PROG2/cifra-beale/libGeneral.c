#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "libGeneral.h"

char* cypherBookNumbering(FILE *cypherBook)
{
    /* ... */
}

char* keysBookCreation(FILE *cypherBook)
{
    /* ... */
}

int freeAllocatedMemory(FILE *cypherBook)
{
    fclose(cypherBook);
}

int randomNum(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}