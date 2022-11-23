#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{ 
    /* verifica se todos argumentos foram chamados */
    if (argc < 6){
        printf("Chamada errada.\n");
        return 1;
    }

    /* ./lattes -d /lattes-cvs -c qualis-conf.txt -p qualis-periodicos.txt */

    FILE *arq;
    int option, tam;
    char *buffer_conf, *buffer_period, *dir_path;

    while ((option = getopt(argc, argv, "d:c:p:")) != -1){
        switch (option)
        {
        case 'd' :
            dir_path = optarg;
            puts(dir_path);
            break;
        
        case 'c' :
            arq = fopen(optarg, "r");
            if (! arq){
                printf("Erro ao abrir %s\n", optarg);
                exit(1);
            }

            if (arq){
                fseek(arq, 0, SEEK_END);
                tam = ftell(arq);
                fseek(arq, 0, SEEK_SET);
                buffer_conf = malloc ((sizeof(char) * tam) + 1);
                if (buffer_conf)
                    fread(buffer_conf, 1, tam, arq);
            }
            fclose(arq);
            /* puts(buffer_conf); */
            break;
        
        case 'p' :
            arq = fopen(optarg, "r");
            if (! arq){
                printf("Erro ao abrir %s\n", optarg);
                exit(1);
            }

            if (arq){
                fseek(arq, 0, SEEK_END);
                tam = ftell(arq);
                fseek(arq, 0, SEEK_SET);
                buffer_period = malloc ((sizeof(char) * tam) + 1);
                if (buffer_period)
                    fread(buffer_period, 1, tam, arq);
            }
            fclose(arq);
            /* puts(buffer_period); */
            break;

        default:
            printf("Sem opção de chamada.\n");
            exit(1);
            break;
        }
    }
        
    // Leio do curriculo, guardo o periodico numa string.

    char *periodicos_cvs;

    

}