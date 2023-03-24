#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#include "libGeneral.h"
#include "libAVL.h"

/* ------------------ Prototypes ------------------------ */

int randomNum(int min, int max);
void handleEntry(int argc, char **argv, int encryptingMode, int encrypting_CipherBook, int decryptingMode, int decryptingMode_CipherBook, int decryptingMode_KeysFile, 
                    char *originalMsgPath, char *outputPath, char *keysFilePath, char *cipherBookPath);
node_t *cypherBookToTree(char *cipherBookPath, node_t *rootCipherBook);

/* ------------------ Funções Externas ------------------ */

// Retorna um número aleatório em um range de min a max
int randomNum(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

/*
    Oque eu quero que ele faça quando receber entradas:
        Encrypting:
            -b Recebe o LivroCifra e vai lendo ele pegando a primeira letra de cada palavra, ao pegar marca sua posição
        e coloca em um nodo na arvore.
            -m Guarda o nome/caminho do arquivo em uma string.
            -o Guarda o nome/caminho do Output em uma string.
            -c Guarda o nome/caminho do Arquivo de Chaves
*/
void handleEntry(int argc, char **argv, int encryptingMode, int encrypting_CipherBook, int decryptingMode, int decryptingMode_CipherBook, int decryptingMode_KeysFile, 
                    char *originalMsgPath, char *outputPath, char *keysFilePath, char *cipherBookPath)
{
    int option;

    encryptingMode = encrypting_CipherBook = decryptingMode = decryptingMode_CipherBook = decryptingMode_KeysFile = 0;

    printf("Processando as entradas...\n");
    /* 
        encrypt :               ./beale -e -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves
        decrypt :              
            arquivo de chaves:  ./beale -d -i MensagemCodificada -c ArquivoDeChaves -o MensagemDecodificada
            livro de cifra :    ./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada
    */ 

    while ((option = getopt(argc, argv, "eb:m:o:c:d:i:")) != -1)
    {
        switch (option)
        {
        case 'e':
            encryptingMode = 1;
            break;

        case 'd':
            decryptingMode = 1;
            break;

        case 'b':
            if (decryptingMode)
                decryptingMode_CipherBook = 1;

            cipherBookPath = optarg;
            break;

        case 'm':
            break;

        case 'o':
            break;

        case 'c':
            if (encryptingMode)
                encrypting_CipherBook = 1;

            if (decryptingMode)
                decryptingMode_KeysFile = 1;
            break;

        case 'i':
            break;

        default:
            printf("Sem opção de chamada.\n");
            break;
        }
    }
}

/* ------------------ Funções Internas ------------------ */

node_t *cypherBookToTree(char *cipherBookPath, node_t *rootCipherBook)
{
    
}
