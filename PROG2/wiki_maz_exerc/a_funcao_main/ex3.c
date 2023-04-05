#include <stdio.h>
#include <stdlib.h>

#define STRSIZE 1024

void printAllVar(char **envp)
{
    int ambAmount, i;
    char *token;

    while (envp[ambAmount] != NULL)
        ambAmount++;

    for (i = 0; i <= ambAmount; i++){
        token = strtok(envp[i], '=');
        printf("%s\n", token);
    }
}

void printXVars(int amount)
{
    char curVar[STRSIZE];
}

int main(int argv, char **argc, char **envp)
{
    int option;

    while ((option = getopt (argc, argv, "an:")) != -1)
    {
        switch (option)
        {
        case 'a':
            printAllVar(envp);
            break;

        case 'n':
            printXVars(optarg);
            break;

        default:
            printf("Erro de inserção.\n");
            break;
        }
    }
    
}