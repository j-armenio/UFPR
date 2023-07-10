#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "libutil.h"

int main(int argc, char **argv)
{
    if (argc <= 1){
        printf("Erro na execução do comando, insira uma opção.\n");
        return 1;
    }

    int option;

    while ((option = getopt(argc, argv, "ti:a:m:x::r:ch")) != -1)
    {
        switch (option)
        {
        case 'i':
            printf("Insere membros no archive\n");
            insertFilesToBackup(argc, argv);
            break;
        
        case 'a':
            printf("Insere e substitui se for mais recente\n");
            insertIfNew(argc, argv);
            break;

        case 'm':
            printf("Move membro\n");
            moveMember(argc, argv);
            break;

        case 'x':
            printf("Extrai membros\n");
            extract(argc, argv);
            break;

        case 'r':
            printf("Remove membros indicados\n");
            removeFiles(argc, argv);
            break;

        case 'c':
            listMembers(argc, argv);
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