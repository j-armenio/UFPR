#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <getopt.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    int option;

    printf("Tratando as entradas...");

    /* 
        encrypt :                   -e -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves 
        decrypt :                   -d -i MensagemCodificada -c ArquivoDeChaves -o MensagemDecodificada
        input file/livro de cifra : -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada 
    */

    while ((option = getopt(argc, argv, "")) != -1){
        switch (option)
        {
        case /* constant-expression */:
            /* code */
            break;
        
        default:
            break;
        }
    }

    
}