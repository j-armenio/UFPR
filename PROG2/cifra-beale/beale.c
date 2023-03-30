#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#include "libgeneral.h"
#include "libtest.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    char *locale;
    locale = setlocale (LC_CTYPE, "");

    entryInfo_t *inInfo = NULL;

    inInfo = handleEntries(argc, argv);

    if (inInfo->encryptingMode){
        encryptMsg(inInfo);
        printf("Done!\n");
    } else if (inInfo->decryptingMode){
        if (inInfo->decryptingMode_KeysFile){
            decryptMsgWithKeysFile(inInfo);
            printf("Done!\n");
        }
        else if (inInfo->decryptingMode_CipherBook){
            decryptMsgWithCipherBook(inInfo);
            printf("Done!\n");
        }
    }
}