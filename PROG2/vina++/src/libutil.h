#ifndef _LIB_UTIL_
#define _LIB_UTIL_

#include "libmember.h"
#include "liboptions.h"

char *getRelativePath(char *path);
int checkIfFileExists(char *path);
char *getOctalMode(int permissions);
void removeExtension(char *filename);
void createDirectories(char *path);
void copyBinary(FILE *source, int fileSize, FILE *target);
void shiftRight(FILE *file, int blockStart, int blockEnd, int writeEnd);
void shiftLeft(FILE *file, int bStart, int bEnd, int writeStart);
void pasteMember(FILE *bkp, int copyStart, int size, int writeStart);

#endif