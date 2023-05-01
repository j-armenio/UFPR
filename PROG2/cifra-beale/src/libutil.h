#ifndef _LIB_UTIL_H_
#define _LIB_UTIL_H_

#include "liblist.h"

#define ARRAY_SIZE 1024

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

entryInfo_t *allocateFlags();
void destroyEntry(entryInfo_t *inInfo);
listLetters_t *keysFileToList(char *keysFilePath);
int bookToKeysFile(char *keysFilePath, listLetters_t *cipherBookList);
listLetters_t *cipherBookToList(char *cipherBookPath);
int getRandomNumber(int min, int max);
entryInfo_t *handleEntries(int argc, char **argv);


#endif