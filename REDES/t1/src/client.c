#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sys/time.h>
#include <sys/time.h>

#include "message.h"
#include "utils.h"
#include "operations.h"
        
int main() {
    char *interface = CLIENT_INTERFACE;
    int sockfd = create_raw_socket(interface);

    char input[INPUT_SIZE];
    char command[COMMAND_SIZE];
    char filename[FILENAME_SIZE] = "";
    FILE* file;
     int operation = -1;
    
    int running = 1;   // Controle do loop principal

    printf("Sistema iniciado. Digite um comando (backup, restaura, verifica, sair) e o arquivo:\n");

    while (running) {
        char rawPath[INPUT_SIZE] = CURRENT_DIR;

        printf("> ");
        if (fgets(input, sizeof(input), stdin) != NULL) {

            if (strlen(input) == 0) {
                printf("Erro: nenhum comando foi digitado. Tente novamente.\n");
                continue;
            }

            // Remover o caractere de nova linha no final do comando
            input[strcspn(input, "\n")] = '\0';

            // Divide input
            int items = sscanf(input, "%s %s", command, filename);

            // Verificar se nenhum comando foi fornecido
            if (items < 1) {
                printf("Erro: nenhum comando foi digitado. Tente novamente.\n");
                continue;
            }

            else if (items < 2 && strcmp(command, "sair") != 0) {
                printf("Erro: nenhum arquivo foi requisitado. Tente novamente.\n");
                continue;
            }
            
            uint8_t command_code = get_command_code(command);

            switch (command_code) {
                case BACKUP:
                    printf("Executando backup...\n");
                    partFileAndSend(filename, sockfd, BACKUP);
                    partFileAndSend(filename, sockfd, SIZE);
                    partFileAndSend(filename, sockfd, DATA);
                    partFileAndSend(filename, sockfd, END);
                    break;                
                case RESTORE:
                    printf("Executando restauração...\n");
                    partFileAndSend(filename, sockfd, RESTORE);
                    recvPkgAndAssemble(rawPath, sockfd, file, &operation);
                    strcat(rawPath, filename);
                    file = fopen(rawPath, "wb");
                    if (file == NULL) {
                        perror("Erro ao abrir o arquivo para escrita");
                        exit(EXIT_FAILURE);
                    }
                    while(operation != END){
                        recvPkgAndAssemble(rawPath, sockfd, file, &operation);
                    }
                    break;
                case VERIFY:
                    printf("Verificando...\n");
                    partFileAndSend(filename, sockfd, VERIFY);
                    strcat(rawPath, filename);
                    recvPkgAndAssemble(rawPath, sockfd, NULL, &operation);
                    break;
                case EXIT:
                    printf("Encerrando sistema...\n");
                    running = 0;
                    break;

                default:
                    printf("Comando não reconhecido: %s\n", command);
                    break;
            }
        } else {
            printf("Erro ao ler comando. Tente novamente.\n");
        }
    }

    close(sockfd);

    return 0;
}