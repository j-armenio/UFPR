#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

#define TIMEOUT_MILLIS 2000
#define FILENAME_SIZE 100

void partFileAndSend(const char* arquivo, int sockfd, int operation);
void recvPkgAndAssemble(char* outputFile, int sockfd, FILE* file, int* operation);

#endif