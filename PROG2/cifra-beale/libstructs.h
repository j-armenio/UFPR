#ifndef _LIB_STRUCTS_H_
#define _LIB_STRUCTS_H_

#include <stdio.h>

typedef struct entryFlags {
    int encryptingMode;
    int encrypting_CipherBook;

    int decryptingMode;
    int decryptingMode_CipherBook;
    int decryptingMode_KeysFile;

    char *originalMsgPath;
    char *outputPath;
    char *keysFilePath;
    char *cipherBookPath;
} entryFlags_t;

#endif