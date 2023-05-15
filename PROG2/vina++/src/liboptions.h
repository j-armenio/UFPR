#ifndef _LIB_OPTIONS_
#define _LIB_OPTIONS_

#include "libdirectory.h"

int printHelpMessage();
int insertFilesIntoBackup(int argc,char **argv);
void extractFiles(int argc, char **argv);
void extractAllFiles(char *directoryPath);

#endif