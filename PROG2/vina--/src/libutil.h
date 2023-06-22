#ifndef _LIB_UTIL_
#define _LIB_UTIL_

#include "libmember.h"
#include "liboptions.h"

char *getRelativePath(char *path);
int checkIfFileExists(char *path);
char *getOctalMode(int permissions);
void removeExtension(char *filename);
void createDirectories(char *path);

#endif