#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

#include "message.h"
#include "utils.h"
#include "operations.h"

long long timestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec*1000 + tp.tv_usec/1000;
}

void fillPackage(Message* msg, uint8_t seq, uint8_t tam, uint8_t type, unsigned char* tempBuffer){
        msg->MI = INIT_MARKER;
        setSeq(&msg->Header, seq);
        setTam(&msg->Header, tam);
        setType(&msg->Header, type);
        memcpy(msg->Data, tempBuffer, tam);
        msg->CRC = calculateCRC(*msg);
}

int create_raw_socket(char *web_interface_name)
{
    // cria um socket de nível de enlace
    // AF_PACKET: permite capturar pacotes diretamente no nível de Ethernet
    // SOCK_RAW: indica que é um socket sem protocolo
    // htons(ETH_P_ALL): define que todos protocolos de camada 3 serão capturados
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // retorna o identificador do socket
    if (sockfd == -1) {
        fprintf(stderr, "Erro ao criar socket: Verifique se você é root\n");
        exit(-1);
    }

    // obtem o índice da interface de rede
    int ifindex = if_nametoindex(web_interface_name);

    // struct usada para associar o socket a uma interface de rede no nível de enlace de dados (nível 2)
    // Define que o socket usará a interface identificada por ifindex e escutará todos protocolos
    // struct inicializa com 0 para garantir que esteja "limpa"
    struct sockaddr_ll address = {0};
    address.sll_family = AF_PACKET;
    address.sll_protocol = htons(ETH_P_ALL);
    address.sll_ifindex = ifindex;

    // bind associa o socket a interface de rede especificada, permitindo que envie e receba pacotes por essa interface
    if (bind(sockfd, (struct sockaddr*) &address, sizeof(address)) == -1) {
        fprintf(stderr, "Erro ao fazer bind no socket\n");
        exit(-1);
    }

    // Promiscuous mode
    struct packet_mreq mr = {0};
    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    if (setsockopt(sockfd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr, sizeof(mr)) == -1) {
        fprintf(stderr, "Erro ao fazer setsockopt: "
            "Verifique se a interface de rede foi especificada corretamente.\n");
        exit(-1);
    }

    return sockfd;
}

uint8_t calculateCRC(Message msg) {
    uint8_t crc = 0x00;

    // Calcula CRC para o cabeçalho
    crc ^= msg.Header;
    for (int i = 0; i < 8; i++) {
        if (crc & 0x80) {
            crc = (crc << 1) ^ CRC_POLYNOMIAL;
        } else {
            crc <<= 1;
        }
    }

    // Calcula CRC para os dados
    for (int i = 0; i < getTam(msg.Header); i++) {
        crc ^= msg.Data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

int isValidPackage(Message msg, int bytesReceived, int expectedSeq) {

    // Verifica se o pacote está incompleto
    if (bytesReceived != sizeof(msg)) {
        //printf("Pacote incompleto. Enviando NACK para seq: %d\n", getSeq(msg.Header));
        return 0;
    }

    // Verifica a sequência do pacote recebido
    if (getSeq(msg.Header) != expectedSeq) {
        //printf("Pacote fora de sequência. Enviando NACK para seq: %d\n", getSeq(msg.Header));
        return 0;
    }

    // Verifica CRC
    if (msg.CRC != calculateCRC(msg)) {
        //printf("Pacote inválido. Enviando NACK para seq: %d\n", getSeq(msg.Header));
        return 0;
    }

    // Obtém o tamanho real dos dados no pacote recebido
    if (getTam(msg.Header) > MAX_DATA_SIZE) {
        return 0;
    }

    return 1;
}

// Função para mapear os comandos para inteiros
uint8_t get_command_code(const char *command) {
    if (strcmp(command, "backup") == 0) {
        return BACKUP;
    } else if (strcmp(command, "restaura") == 0) {
        return RESTORE;
    } else if (strcmp(command, "verifica") == 0) {
        return VERIFY;
    } else if (strcmp(command, "sair") == 0) {
        return EXIT;
    }
    return -1; // Comando inválido
}

const char* getFileName(const char* filePath) {
    const char* lastSlash = strrchr(filePath, '/');
    if (lastSlash) {
        return strdup(lastSlash + 1);
    }
    return filePath;
}

void calculateChecksum(const char *filename, uint32_t* checksum) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }

    *checksum = 0;
    unsigned char buffer[1024];
    size_t bytesRead;

    // Lê o arquivo em blocos e soma os valores dos bytes
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytesRead; i++) {
            *checksum += buffer[i]; 
        }
    }

    fclose(file);
}

// Função para lidar com a escrita no arquivo
void receiveData(Message *msg, FILE *file, Message *response) {
    fwrite(msg->Data, 1, getTam(msg->Header), file);
    fflush(file);
    setType(&response->Header, ACK);
}

// Função para manipular o nome do arquivo
void receiveFilename(char *filename, Message *msg) {
    if (strlen(filename) + getTam(msg->Header) < FILENAME_SIZE) {
        strncat(filename, (char *)msg->Data, getTam(msg->Header));
    } else {
        fprintf(stderr, "Erro: nome do arquivo excede o tamanho máximo permitido.\n");
        exit(EXIT_FAILURE);
    }
}

// Função para verificar espaço em disco
void receiveSize(Message *msg, Message *response) {
    uint32_t fileSize;
    memcpy(&fileSize, msg->Data, getTam(msg->Header));
    setType(&response->Header, OK);
    
}

void receiveChecksum(Message* msg, Message* response, char** outputFile){
    uint32_t checkSumServer, checkSumClient;
    memcpy(&checkSumServer, msg->Data, getTam(msg->Header));
    calculateChecksum(*outputFile, &checkSumClient);

    if (checkSumServer == checkSumClient) {
        printf("Os arquivos são iguais!\n");
    } else {
        printf("Os arquivos são diferentes!\n");
    }
    setType(&response->Header, OK);
}

void sendData(FILE *file, Message *msg, uint8_t *seq, uint8_t *bytesRead, unsigned char *tempBuffer) {
    *bytesRead = fread(tempBuffer, 1, MAX_DATA_SIZE, file);
    fillPackage(msg, *seq, *bytesRead, DATA, tempBuffer);
}

void sendFilename(const char *fileName, int *offset, int fileNameSize, Message *msg, uint8_t *seq, uint8_t *bytesRead, unsigned char *tempBuffer) {
    *bytesRead = (fileNameSize - *offset > MAX_DATA_SIZE) ? MAX_DATA_SIZE : (fileNameSize - *offset);
    memcpy(tempBuffer, fileName + *offset, *bytesRead);
    *offset += *bytesRead;
    fillPackage(msg, *seq, *bytesRead, getType(msg->Header), tempBuffer);
}

void sendSize(const char *fileName, Message *msg, uint8_t *seq, uint8_t *bytesRead) {
    if (*seq == 1) return;

    struct stat fileStat;
    if (stat(fileName, &fileStat) < 0) {
        perror("Erro ao obter informações do arquivo");
        exit(EXIT_FAILURE);
    }
    uint32_t fileSize = fileStat.st_size;
    memcpy(msg->Data, &fileSize, sizeof(fileSize));
    *bytesRead = 1; // Para evitar leitura adicional
    fillPackage(msg, *seq, sizeof(fileSize), SIZE, msg->Data);
}

void sendChecksum(const char *fileName, Message *msg, uint8_t *seq, uint8_t *bytesRead) {
    if (*seq == 1) return;

    uint32_t checkSum;
    calculateChecksum(fileName, &checkSum);
    memcpy(msg->Data, &checkSum, sizeof(checkSum));
    *bytesRead = 1; // Para evitar leitura adicional
    fillPackage(msg, *seq, sizeof(checkSum), OKCHECKSUM, msg->Data);
}

void configureTimeout(int sockfd) {
    struct timeval timeout = { .tv_sec = TIMEOUT_MILLIS / 1000, .tv_usec = (TIMEOUT_MILLIS % 1000) * 1000 };
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}
