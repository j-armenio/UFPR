#ifndef _LIB_MEMUTIL_
#define _LIB_MEMUTIL_

#include "libmember.h"

void updateListPositions(directory *dir);
void updatePositionsBkp(FILE *bkp, directory *dir);
void updateBkpPositions(directory *dir);
int getFilesTotalSize(directory *dir);
member *getMemberByPositionInList(directory *dir, int positionList);
member *getMemberByPath(directory *dir, char *path);
void writeMember(FILE *bkp, member *m);
void printMember(member *member);
void printDirectory(directory *dir);

#endif