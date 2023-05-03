#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "liboptions.h"

int main(int argc, char *argv[])
{
    int option;

    while ((option = getopt(argc, argv, "i:a:m:x::r:ch")) != -1)
    {
        switch (option)
        {
        case 'i':
            break;
        
        case 'a':
            break;

        case 'm':
            break;

        case 'x':
            break;

        case 'r':
            break;

        case 'c':
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