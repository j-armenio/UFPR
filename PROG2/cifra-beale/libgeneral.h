#ifndef _LIB_GENERAL_H_
#define _LIB_GENERAL_H_

#include <stdio.h>

#include "libavl.h"

typedef struct entryInfo {
    int encryptingMode; // OK
    int encrypting_CipherBook; // OK

    int decryptingMode; // OK
    int decryptingMode_CipherBook; // OK
    int decryptingMode_KeysFile; // OK

    char *originalMsgPath; // OK
    char *outputPath; // OK
    char *keysFilePath; 
    char *cipherBookPath;
} entryInfo_t;

int randomNum(int min, int max);
void handleEntries(int argc, char **argv, entryInfo_t *inInfo);

#endif