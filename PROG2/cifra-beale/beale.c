#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libGeneral.h"

int main(int argc, char **argv)
{
    int option, encrypting, decrypting;
    char *originalMsg, *outputFile, *keysFile;
    FILE *cypherBook;

    encrypting = decrypting = 0;

    printf("Processando as entradas...\n");

    /* 
        encrypt :                   ./beale -e -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves 
        decrypt :                   ./beale -d -i MensagemCodificada -c ArquivoDeChaves -o MensagemDecodificada
        input file/livro de cifra : ./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada 
    */

    while ((option = getopt(argc, argv, "eb:m:o:c:d:i:")) != -1){
        switch (option)
        {
        case 'e':
            encrypting = 1;
            break;
        case 'd':
            decrypting = 1;
            break;
        case 'b':
            cypherBook = fopen(optarg, "r");
            /* Pega o livro cifra e isola a primeira letra de cada palavra do livro para
               um elemento de um array */
            break;
        case 'm':
            if (encrypting)
                originalMsg = optarg;
            break;
        case 'o':
            outputFile = optarg;
            break;
        case 'c':
            keysFile = optarg;
        case 'i':
            if (decrypting)
                originalMsg = optarg;
            break;
        default:
            printf("Sem opção de chamada.\n");
            break;
        }
    }

    if (encrypting){

    } else {

    };

    freeAllocatedMemory();
}