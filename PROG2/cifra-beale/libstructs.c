#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#include "libstructs.h"

entryFlags_t *allocateFlags()
{
    entryFlags_t *ret = NULL;

    ret = (entryFlags_t *)malloc(sizeof(entryFlags_t));
    if (! ret){
        printf("Falha ao alocar flags de entrada.\n");
        return NULL;
    }
}