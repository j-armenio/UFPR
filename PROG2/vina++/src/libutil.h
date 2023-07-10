#ifndef _LIB_UTIL_
#define _LIB_UTIL_

#include "libmember.h"
#include "liboptions.h"
#include "libbinary.h"

char *getRelativePath(char *path);
int checkIfFileExists(char *path);
char *getOctalMode(int permissions);
void removeExtension(char *filename);
void createDirectories(char *path);
void shiftRight(FILE *file, int blockStart, int blockEnd, int writeEnd);
void shiftLeft(FILE *file, int bStart, int bEnd, int writeStart);

#endif