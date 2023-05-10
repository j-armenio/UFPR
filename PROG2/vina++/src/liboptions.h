#ifndef _LIB_OPTIONS_
#define _LIB_OPTIONS_

#include "libdirectory.h"

int printHelpMessage();
int insertFilesIntoBackup(int argc,char **argv);
member_t *fillMemberInfo(char *currentPath);

#endif