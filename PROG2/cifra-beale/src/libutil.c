#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <wchar.h>

#include "libutil.h"

/* ------------------ PR0TOTYPES ------------------ */

entryInfo_t *allocateFlags();
void destroyEntry(entryInfo_t *inInfo);
listLetters_t *keysFileToList(char *keysFilePath);
int bookToKeysFile(char *keysFilePath, listLetters_t *cipherBookList);
listLetters_t *cipherBookToList(char *cipherBookPath);
int getRandomNumber(int min, int max);
entryInfo_t *handleEntries(int argc, char **argv);

/* ------------------ FUNCTIONS ------------------ */

/* Retorna um número aleatório em um range de min a max */
int getRandomNumber(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

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

    return inInfo;
}

void destroyEntry(entryInfo_t *inInfo)
{
    free(inInfo);
}

listLetters_t *keysFileToList(char *keysFilePath)
{
    /* 1. Ler o arquivo de chaves e armazenar na lista; */
    FILE *keysFile = NULL;
    listLetters_t *list = NULL;

    keysFile = fopen(keysFilePath, "r");
    if (! keysFile){
        printf("Falha ao abrir o arquivo de chaves.\n");
        return NULL;
    }

    list = createLettersList();

    char c;
    char line[1024];

    char s[2] = " ";
    char *token;
    

    while (! feof (keysFile))
    {
        /* uso o fgetc pra pegar a primeira letra */
        c = fgetc(keysFile);
        insertLetter(c, list);

        /* uso o fgets pra pegar o resto da linha */
        fgets(line, 1024, keysFile);

        token = strtok(line, s);

        while (token != NULL){
            token = strtok(NULL, s);
            if (token != NULL && token[0] != ' ')
                if (isalnum(token[0]))
                    insertPosition(atoi(token), searchLetter(c, list));
        }
    }

    /* printList(list); */

    return list;
}

entryInfo_t *handleEntries(int argc, char **argv)
{
    int option;
    entryInfo_t *inInfo = NULL;

    inInfo = allocateFlags();

    inInfo->encryptingMode = inInfo->encrypting_KeysFile = inInfo->decryptingMode = 
        inInfo->decryptingMode_CipherBook = inInfo->decryptingMode_KeysFile = 0;

    printf("Processing the command line...\n");

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

listLetters_t *cipherBookToList(char *cipherBookPath)
{
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
        if (position == 0){
            while (! isalnum(c)){
                c = fgetc(cipherBook);
            }
            insertPosition(position, searchLetter(tolower(c), list));
            position++;
            prev_c = c;
            c = fgetc(cipherBook);
            continue;
        }

        if (isalnum(c) && ((prev_c == ' ') || (prev_c == '\n')))
        {
            insertPosition(position, searchLetter(tolower(c), list));
            position++;
        }
        prev_c = c;
    }

    fclose(cipherBook);

    return list;
}

int bookToKeysFile(char *keysFilePath, listLetters_t *cipherBookList)
{
    printf("Asking for keys file - Converting cipher book to keys file...\n");

    if (! printListToFile(cipherBookList, keysFilePath))
        return 0;
    return 1;
}