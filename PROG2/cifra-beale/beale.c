#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libGeneral.h"
#include "libAVL.h"

int main(int argc, char **argv)
{
    int encryptingMode, encrypting_CipherBook, decryptingMode, decryptingMode_KeysFile, decryptingMode_CipherBook;
    char *originalMsgPath, *outputPath, *keysFilePath, *cipherBookPath;
    node_t *rootCipherBook = NULL;

    handleEntries(argc, argv, encryptingMode, encrypting_CipherBook, decryptingMode, decryptingMode_CipherBook, decryptingMode_KeysFile, originalMsgPath, outputPath, keysFilePath);

    if (encryptingMode)
    {
        encrypt(rootCipherBook, originalMsgPath, outputPath);
    }
    else if (decryptingMode)
    {
        decrypt(rootCipherBook, originalMsgPath, outputPath);
    }
}