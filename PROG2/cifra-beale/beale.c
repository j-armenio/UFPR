#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgeneral.h"
#include "libavl.h"

int main(int argc, char **argv)
{
    entryInfo_t *inInfo = NULL;
    node_t *rootCipherBook = NULL;

    handleEntries(argc, argv, inInfo);

    if (inInfo->encryptingMode){
        
    }
}