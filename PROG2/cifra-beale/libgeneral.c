#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#include "libgeneral.h"
#include "libavl.h"

/* ------------------ Prototypes ------------------------ */

int randomNum(int min, int max);
void handleEntry(int argc, char **argv, entryInfo_t *entryFlags);
node_t *cypherBookToTree(char *cipherBookPath, node_t *rootCipherBook);
entryInfo_t *allocateFlags();
void destroyEntry(entryInfo_t *inInfo);

/* ------------------ Funções Internas ------------------ */

entryInfo_t *allocateFlags()
{
    entryInfo_t *inInfo = NULL;

    inInfo = (entryInfo_t *)malloc(sizeof(entryInfo_t));
    if (! inInfo){
        printf("Falha ao alocar flags de entrada.\n");
        return NULL;
    }

    inInfo->encryptingMode = 0;
    inInfo->encrypting_CipherBook = 0;
    inInfo->decryptingMode = 0;
    inInfo->decryptingMode_CipherBook = 0;
    inInfo->decryptingMode_KeysFile = 0;

    inInfo->keysFilePath = NULL;
    inInfo->cipherBookPath = NULL;
    inInfo->originalMsgPath = NULL;
    inInfo->outputPath = NULL;
}

void destroyEntry(entryInfo_t *inInfo)
{
    free(inInfo);
}


/* ------------------ Funções Externas ------------------ */

// Retorna um número aleatório em um range de min a max
int randomNum(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

void handleEntries(int argc, char **argv, entryInfo_t *inInfo)
{
    int option;

    inInfo = allocateFlags();

    inInfo->encryptingMode = inInfo->encrypting_CipherBook = inInfo->decryptingMode = 
        inInfo->decryptingMode_CipherBook = inInfo->decryptingMode_KeysFile = 0;

    printf("Processando as entradas...\n");
    /* 
        encrypt :               ./beale -e -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves
        decrypt :              
            arquivo de chaves:  ./beale -d -i MensagemCodificada -c ArquivoDeChaves -o MensagemDecodificada
            livro de cifra :    ./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada
    */ 

    while ((option = getopt(argc, argv, "eb:m:o:c:di:")) != -1)
    {
        switch (option)
        {
        case 'e':
            inInfo->encryptingMode = 1;
            break;

        case 'd':
            inInfo->decryptingMode = 1;
            break;

        case 'b':
            if (inInfo->decryptingMode)
                inInfo->decryptingMode_CipherBook = 1;
            
            if (inInfo->encryptingMode)
                inInfo->encrypting_CipherBook = 1;

            inInfo->cipherBookPath = optarg;
            break;

        case 'm':
            if (inInfo->encryptingMode)
                inInfo->originalMsgPath = optarg;
            break;

        case 'i':
            inInfo->originalMsgPath = optarg;
            break;

        case 'o':
            inInfo->outputPath = optarg;
            break;

        case 'c':
            inInfo->keysFilePath = optarg;

            if (inInfo->decryptingMode)
                inInfo->decryptingMode_KeysFile = 1;
            break;

        default:
            printf("Sem opção de chamada.\n");
            break;
        }
    }
}