#ifndef _LIB_GENERAL_H_
#define _LIB_GENERAL_H_

#include <stdio.h>

void processCommandLine(int argc, char *argv[], int encrypting, int decrypting, char *originalMsg, char *outputFile, 
                        char *keysFile, FILE *cypherBook);
char* cypherBookNumbering(FILE *cypherBook);
char* keysBookCreation(FILE *cypherBook);
int freeAllocatedMemory(FILE *cypherBook);

#endif