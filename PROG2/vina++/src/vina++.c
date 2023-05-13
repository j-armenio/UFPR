#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "liboptions.h"
#include "libtest.h"

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
            insertFilesIntoBackup(argc, argv);
            break;
        
        case 'a':
            printf("opcao a\n");
            break;

        case 'm':
            printf("move o membro indicado para o target");
            break;

        case 'x':
            printf("extrai os membros indicados\n");
            break;

        case 'r':
            printf("remove membros indicados\n");
            break;

        case 'c':
            printf("lista conteudo em ordem\n");
            break;

        case 'h':
            printHelpMessage();
            break;

        case 't':
            printf("Caso para testes: \n");
            doTest(argc, argv);
            break;

        default:
            printf("Erro na inserção do comando.\n");
            return 1;
            break;
        }
    }

    return 0;
}