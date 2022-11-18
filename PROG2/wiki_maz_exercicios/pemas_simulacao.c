#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#define LINESIZE 1024

/*
    Abra a pasta poemas e leia todos arquivos. Conte a quantidade de vezes que as palavra "amor" e "céu" foram ditas.
*/

int main(){
    int cont_ceu, cont_amor;
    DIR *dirstream;
    struct dirent *direntry;
    FILE *arq;
    char c;
    int i;
    char *needle1 = "amor";
    char *needle2 = "céu";


    dirstream = opendir("./poemas");
    if (!dirstream){
        perror("Erro ao abrir pasta poemas.\n");
        exit(1);
    }

    for (;;){
        direntry = readdir(dirstream);
        if (!direntry)
            break;

        arq = fopen(direntry->d_name, "r");

        while((c = fgetc(arq)) != EOF){
            if(c == *needle1){ 
                for(i = 0; i < strlen(needle1); i ++){ 
                    c = fgetc(arq); 
                    if(c == EOF) 
                        break;
                    if(*(needle1 + i) != c) 
                        break;
                }
                if(i == strlen(needle1)){ 
                        cont_amor++;
                        break;
                }
            }
        }
    }
    fclose(arq);
    (void) closedir(dirstream);
    exit(0);
}