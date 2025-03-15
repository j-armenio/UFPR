#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <openssl/md5.h>
#include <sys/stat.h>

#include "message.h"
#include "utils.h"
#include "operations.h"

int main()
{
    FILE* file = NULL;
    struct stat fileStat;
    char *interface = SERVER_INTERFACE;
    char outputfile[FILENAME_SIZE] = "";
    int sockfd = create_raw_socket(interface);
    int operation = -1;
    int running = 1;

    // Recebe comando do cliente, operação e nome do arquivo por ponteiro
    while(running){
        char rawPath[INPUT_SIZE] = SERVER_BACKUP;
        printf("\nEsperando comando do cliente...\n");
        recvPkgAndAssemble(outputfile, sockfd, file, &operation);
        //Adiciona caminho para pasta de backup "server"
        strcat(rawPath, outputfile);
        switch(operation){
            case BACKUP:
            printf("Recebendo arquivo do cliente...\n");
                file = fopen(rawPath, "wb");
                if (file == NULL) {
                    perror("Erro ao abrir o arquivo para escrita");
                }
                while(operation != END){
                    recvPkgAndAssemble(rawPath, sockfd, file, &operation);
                }
                break;
            case RESTORE:
                printf("Enviando arquivo para o cliente...\n");
                if (stat(rawPath, &fileStat) < 0) {
                    printf("Arquivo não encontrado: %s\n", rawPath);
                    exit(EXIT_FAILURE);
                }
                partFileAndSend(rawPath, sockfd, SIZE);
                partFileAndSend(rawPath, sockfd, DATA);
                partFileAndSend(rawPath, sockfd, END);
                break;
            case VERIFY:
                printf("Verificando integridade dos arquivos...\n");
                partFileAndSend(rawPath, sockfd, OKCHECKSUM);
                break;
            default:
                running = 0;
                break;
        }
    }
    
    close(sockfd);

    return 0;
}