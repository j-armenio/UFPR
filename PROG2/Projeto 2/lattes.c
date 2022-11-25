#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

/* ############################# Funções ###################################### */

char *file_to_buffer(char *file_name)
{
    FILE *file;
    char *buffer;
    int tam;
    
    file = fopen(file_name, "r");
    if (! file){
        printf("Failed to open %s\n", file_name);
        exit(1);
    }

    if (file){
        fseek(file, 0, SEEK_END);
        tam = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc ((sizeof(char) * (tam + 1)));
        if (buffer)
            fread(buffer, 1, tam, file);
    }
    fclose(file);
    return buffer;
}

/* ############################# Main ###################################### */

int main(int argc, char **argv)
{ 
    /* verifica se todos argumentos foram chamados */
    if (argc < 6){
        printf("Chamada incorreta.\n");
        return 1;
    }

    /* ./lattes -d lattes-cvs -c qualis-conf.txt -p qualis-periodicos.txt */

    int option;
    char *buffer_conf, *buffer_period, *dir_path;

    while ((option = getopt(argc, argv, "d:c:p:")) != -1){
        switch (option)
        {
        case 'd' :
            dir_path = optarg;
            break;
        
        case 'c' :
            buffer_conf = file_to_buffer(optarg);
            break;

        case 'p' :
            buffer_period = file_to_buffer(optarg);
            break;

        default:
            printf("Sem opção de chamada.\n");
            exit(1);
            break;
        }
    }

    /* ################################################################### */

    DIR *dirstream;
    struct dirent *direntry;

    dirstream = opendir(dir_path);
    if (! dirstream){
        printf("Falha em abrir o diretorio %s\n", dir_path);
        exit(1);
    }

    /* varre as entradas do diretorio aberto */
    for (;;){
        /* pega a proxima entrada */
        direntry = readdir(dirstream);
        /* criterio de parada: se for nula, para */
        if (! direntry)
            break;

        /* encontra os periodicos no xml e os 
        guarda num vetor de strings(matriz) */
        
        

    }   



    free(buffer_conf);
    free(buffer_period);
}