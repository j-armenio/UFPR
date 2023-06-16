#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "liboptions.h"
#include "libmember.h"
#include "libutil.h"

int main(int argc, char **argv)
{
    /* if (argc <= 1){
        printf("Erro na execução do comando, insira uma opção.\n");
        return 1;
    } */

    int option;

    while ((option = getopt(argc, argv, "ti:a:m:x::r:ch")) != -1)
    {
        switch (option)
        {
        case 'i':
            if (argc <= 3){
                printf("Erro ao inserir o comando.\n");
                return 1;
            }
            printf("Insere membros no archive\n");
            insertFilesToBackup(argc, argv);
            break;
        
        case 'a':
            printf("Opcao -a\n");
            break;

        case 'm':
            printf("Move o membro indicado para o target");
            break;

        case 'x':
            if (argc == 3){
                printf("Extrai todos os membros\n");
                extractAllFiles(argc, argv);
            } else {
                printf("Extrai membros indicados\n");
                extractFiles(argc, argv);
            }
            break;

        case 'r':
            printf("Remove membros indicados\n");
            break;

        case 'c':
            printf("Lista conteudo em ordem\n");
            break;

        case 'h':
            printHelpMessage();
            break;

        default:
            printf("Erro na inserção do comando.\n");
            return 1;
            break;
        }
    }

    return 0;
}