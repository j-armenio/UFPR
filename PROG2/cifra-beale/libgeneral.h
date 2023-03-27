#ifndef _LIB_GENERAL_H_
#define _LIB_GENERAL_H_

#include <stdio.h>

typedef struct entryInfo {
    int encryptingMode;
    int encrypting_KeysFile;

    int decryptingMode;
    int decryptingMode_CipherBook;
    int decryptingMode_KeysFile;

    char *originalMsgPath;
    char *outputPath;
    char *keysFilePath; 
    char *cipherBookPath;
} entryInfo_t;

int randomNum(int min, int max);
entryInfo_t *handleEntries(int argc, char **argv);
int encryptMsg(entryInfo_t *inInfo);
int bookToKeysFile();
int decryptMsgWithKeysFile();
int decryptMsgWithCipherBook();

#endif