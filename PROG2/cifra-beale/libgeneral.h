#ifndef _LIB_GENERAL_H_
#define _LIB_GENERAL_H_

#include <stdio.h>

#include "libAVL.h"

int randomNum(int min, int max);
void handleEntry(int argc, char **argv, int encryptingMode, int encrypting_CipherBook, int decryptingMode, int decryptingMode_CipherBook, int decryptingMode_KeysFile, 
                    char *originalMsgPath, char *outputPath, char *keysFilePath, char *cipherBookPath);

#endif