#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgeneral.h"
#include "libtest.h"

int main(int argc, char **argv)
{
    entryInfo_t *inInfo = NULL;
    FILE *keysFile = NULL;

    inInfo = handleEntries(argc, argv);

    if (inInfo->encryptingMode){
        encryptMsg(inInfo);
        if (inInfo->encrypting_KeysFile){
            bookToKeysFile();
        }
    } else if (inInfo->decryptingMode){
        printf("Decrypting\n");
        if (inInfo->decryptingMode_KeysFile){
            decryptMsgWithKeysFile(inInfo, keysFile);
        }
        else if (inInfo->decryptingMode_CipherBook){
            decryptMsgWithCipherBook(inInfo, keysFile);
        }
    }
}