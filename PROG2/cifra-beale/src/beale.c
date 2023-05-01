#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#include "libutil.h"
#include "libcrypto.h"

int main(int argc, char **argv)
{
    srand(time(NULL));

    entryInfo_t *inInfo = NULL;

    inInfo = handleEntries(argc, argv);

    if (inInfo->encryptingMode)
    {
        encryptMsg(inInfo);
    } 
    else if (inInfo->decryptingMode)
    {
        if (inInfo->decryptingMode_KeysFile)
        {
            decryptMsgWithKeysFile(inInfo);
        }
        else if (inInfo->decryptingMode_CipherBook)
        {
            decryptMsgWithCipherBook(inInfo);
        }
    }

    destroyEntry(inInfo);

    return 0;
}