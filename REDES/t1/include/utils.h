#ifndef UTILS_H
#define UTILS_H

#include "message.h"

#define CLIENT_INTERFACE "lo"
#define SERVER_INTERFACE "lo"

#define CRC_POLYNOMIAL 0xD5

#define INPUT_SIZE 200
#define FILENAME_SIZE 100
#define COMMAND_SIZE 100



#define SERVER_BACKUP "./server/"
#define CURRENT_DIR "./"

long long timestamp();
void fillPackage(Message* msg, uint8_t seq, uint8_t tam, uint8_t type, unsigned char* tempBuffer);
int create_raw_socket(char *web_interface_name);
uint8_t calculateCRC(Message msg);
int isValidPackage(Message msg, int bytesReceived, int expectedSeq);
uint8_t get_command_code(const char *command);
const char* getFileName(const char* filePath);
void calculateChecksum(const char *filename, uint32_t* checksum);
void receiveData(Message *msg, FILE *file, Message *response);
void receiveFilename(char *filename, Message *msg);
void receiveSize(Message *msg, Message *response);
void receiveChecksum(Message* msg, Message* response, char** outputFile);
void sendData(FILE *file, Message *msg, uint8_t *seq, uint8_t *bytesRead, unsigned char *tempBuffer);
void sendFilename(const char *fileName, int *offset, int fileNameSize, Message *msg, uint8_t *seq, uint8_t *bytesRead, unsigned char *tempBuffer);
void sendSize(const char *fileName, Message *msg, uint8_t *seq, uint8_t *bytesRead);
void sendChecksum(const char *fileName, Message *msg, uint8_t *seq, uint8_t *bytesRead);
void configureTimeout(int sockfd);

#endif
