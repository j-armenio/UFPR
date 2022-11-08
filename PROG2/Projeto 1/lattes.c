#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char **argv) // an array of pointers
{
    // verifica se todos argumentos foram chamados
    /*
    if (argc < 6){
        printf("Chamada errada.\n");
        return 1;
    } 
    */

    // forma de chamada:
    // ./lattes -d <diretorio com os CVs> -c <arquivo com a lista Qualis Conf> -p <arquivo com a lista Qualis Periódicos>
    // ./lattes -d /curriculos -c lista_conferencias.txt -p lista_periodicos.txt

    int option, tam;
    FILE *l_conf, *l_period;
    char *buffer_conf, *buffer_period;
    char *dir_curriculos;

    while ((option = getopt(argc, argv, "d:c:p:")) != -1){
        switch (option)
        {
        case 'd' : // diretorio com curriculos
            dir_curriculos = optarg;
            printf("%s\n", optarg);
            break;
        
        case 'c' : // lista conferencias
            l_conf = fopen(optarg, "r");

            if (l_conf){
                fseek(l_conf, 0, SEEK_END);
                tam = ftell(l_conf);
                fseek(l_conf, 0, SEEK_SET);
                buffer_conf = malloc ((sizeof(char) * tam) + 1);
                if (buffer_conf){
                    fread(buffer_conf, 1, tam, l_conf);
                }
            }
            fclose(l_conf);
            break;
        
        case 'p' : // lista periodicos
            l_period = fopen(optarg, "r");

            if (l_period){
                fseek(l_period, 0, SEEK_END);
                tam = ftell(l_period);
                fseek(l_period, 0, SEEK_SET);
                buffer_period = malloc ((sizeof(char) * tam) + 1);
                if (buffer_period){
                    fread(buffer_period, 1, tam, l_period);
                }
            }
            fclose(l_period);
            break;

        default:
            printf("Opção desconhecida.\n");
            exit(1);
            break;
        }
    }

}