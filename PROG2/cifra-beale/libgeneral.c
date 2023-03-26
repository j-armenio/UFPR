#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

#include "libgeneral.h"
#include "liblist.h"

/* ------------------ Prototypes ------------------------ */

int randomNum(int min, int max);
entryInfo_t *handleEntries(int argc, char **argv);
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
    inInfo->encrypting_KeysFile = 0;
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

int normalizeAccent(int inChar)
{
    // 
}


listLetters_t *cipherBookToList(char *cipherBookPath)
{
    char firstLetter;

    FILE *cipherBook = NULL;
    listLetters_t *list = NULL;

    list = createLettersList();
    list = indexAllList(list);

    cipherBook = fopen(cipherBookPath, "r");
    if (! cipherBook){
        printf("Falha ao abrir o livro de cifra.\n");
        return NULL;
    }

    int position = 0;
    int c, prev_c = ' ';

    while ((c = fgetc(cipherBook)) != EOF)
    {
        // c = normalizeAccent(c);
        // First letter of the file
        if (position == 0){
            while (! isalnum(c)){
                c = fgetc(cipherBook);
            }
            // printf("%d: %c \n", position, tolower(c));
            insertPosition(position, searchLetter(tolower(c), list));
            position++;
            prev_c = tolower(c);
            c = fgetc(cipherBook);
            continue;
        }

        if (isalnum(c) && (!isalnum(prev_c) && !(prev_c == '-')))
            {
            // printf("%d: %c  --- %c \n", position, tolower(c), prev_c);
            insertPosition(position, searchLetter(tolower(c), list));
            position++;
        }
        prev_c = tolower(c);
    }
    printf("\n");

    // printList(list);

    fclose(cipherBook);
}

/* ------------------ Funções Externas ------------------ */

// Retorna um número aleatório em um range de min a max
int randomNum(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

entryInfo_t *handleEntries(int argc, char **argv)
{
    int option;
    entryInfo_t *inInfo = NULL;

    inInfo = allocateFlags();

    inInfo->encryptingMode = inInfo->encrypting_KeysFile = inInfo->decryptingMode = 
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

            if (inInfo->encryptingMode)
                inInfo->encrypting_KeysFile = 1;

            if (inInfo->decryptingMode)
                inInfo->decryptingMode_KeysFile = 1;
            break;

        default:
            printf("Sem opção de chamada.\n");
            break;
        }
    }
    
    return inInfo;
}

/* 
    Para criptografar uma mensagem tendo um livro de cifra, o programa deve:
        1. Ler o livro de cifra e armazenar na lista;
        2. Ler a mensagem original;
        3. Criptografar a mensagem original;
        4. Escrever a mensagem criptografada em um arquivo de saída.
*/
int encryptMsg(entryInfo_t *inInfo)
{
    listLetters_t *cipherBookList = NULL;

    printf("Encrypting...\n");

    // 1. Ler o livro de cifra e armazenar na lista;
    cipherBookList = cipherBookToList(inInfo->cipherBookPath);

    // 2. Ler a mensagem original
    convertMsg();
}

int bookToKeysFile()
{
    printf("Asking for keys file - Converting cipher book to keys file...\n");
}

int decryptMsgWithKeysFile()
{
    printf("Decrypting with keys file...\n");
}

int decryptMsgWithCipherBook()
{
    printf("Decrypting with cipher book...\n");
}

