#ifndef _LIB_BINARY_
#define _LIB_BINARY_

#include "libmember.h"

void copyBinary(FILE *source, int fileSize, FILE *target);
void pasteMember(FILE *bkp, int copyStart, int size, int writeStart);
void shiftRight(FILE *file, int bStart, int bEnd, int writeEnd);
void shiftLeft(FILE *file, int bStart, int bEnd, int writeStart);
directory *readBackupToDirectory(FILE *bkp);
void extractContent(FILE *bkp, directory *dirList);

#endif