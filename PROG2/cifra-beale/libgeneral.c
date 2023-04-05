#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <wchar.h>

#include "libgeneral.h"
#include "liblist.h"

/* ------------------ Prototypes ------------------------ */

int getRandomNumber(int min, int max);
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

int bookToKeysFile(char *keysFilePath, listLetters_t *cipherBookList)
{
    printf("Asking for keys file - Converting cipher book to keys file...\n");

    printListToFile(cipherBookList, keysFilePath);    
}

void encryptAndWrite(entryInfo_t *inInfo, listLetters_t* cipherBookList)
{
    FILE *originalMsg = NULL;
    FILE *outputMsg = NULL;

    originalMsg = fopen(inInfo->originalMsgPath, "r");
    if (! originalMsg){
        printf("Falha ao abrir a mensagem original.\n");
        return;
    }

    outputMsg = fopen(inInfo->outputPath, "w");
    if (! outputMsg){
        printf("Falha ao abrir o arquivo de saída.\n");
        return;
    }

    char c;
    int randomNum;
    nodeLetter_t *letterAux = NULL;
    nodePosition_t *positionAux = NULL;

    while ((c = fgetc(originalMsg)) != EOF)
    {
        if (isalnum(c))
        {
            letterAux = searchLetter(tolower(c), cipherBookList);
            positionAux = letterAux->positions;

            randomNum = getRandomNumber(0, letterAux->repetitions);

            while (randomNum > 0 && positionAux->next != NULL)
            {
                positionAux = positionAux->next;
                randomNum--;
            }
            fprintf(outputMsg, "%d ", positionAux->position);
        } else if (c == ' '){
            fprintf(outputMsg, "-1 ");
        } else if (c == '\n'){
            fprintf(outputMsg, "-2 ");
        }
    }

    fclose(originalMsg);
    fclose(outputMsg);
}

void decryptAndWrite(entryInfo_t *inInfo, listLetters_t* cipherBookList)
{
    FILE *originalMsg = NULL;
    FILE *outputMsg = NULL;

    originalMsg = fopen(inInfo->originalMsgPath, "r");
    if (! originalMsg){
        printf("Falha ao abrir a mensagem original.\n");
        return;
    }

    outputMsg = fopen(inInfo->outputPath, "w");
    if (! outputMsg){
        printf("Falha ao abrir o arquivo de saída.\n");
        return;
    }

    char line[1024];
    char *token;

    int i;

    while (! feof (originalMsg))
    {
        fgets(line, 1024, originalMsg);
        if (line[0] == '\0')
            break;
        if (line[0] == ' ')
            continue;
        token = strtok(line, " ");

        while (token != NULL){
            if (atoi(token) == -1){
                fprintf(outputMsg, " ");
            } else if (atoi(token) == -2){
                /* printf("\n"); */
                fprintf(outputMsg, "\n");
            } else {
                /* printf("%c", searchPosition(atoi(token), cipherBookList)->letter); */
                fprintf(outputMsg, "%c", searchPosition(atoi(token), cipherBookList)->letter);
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(originalMsg);
    fclose(outputMsg);
}

/* ------------------ Funções Externas ------------------ */

/* Retorna um número aleatório em um range de min a max */
int getRandomNumber(int min, int max)
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

    printf("Processing the command line...\n");
    /* 
        encrypt :               ./beale -e -b LivroCifra -m MensagemOriginal -o MensagemCodificada -c ArquivoDeChaves
                                ./beale -e -b ./txts/LivroCifra -m ./txts/MensagemOriginal -o ./txts/MensagemCodificada -c ./txts/OUTArquivoDeChaves
        decrypt :              
            arquivo de chaves:  ./beale -d -i MensagemCodificada -c ArquivoDeChaves -o MensagemDecodificada
                                ./beale -d -i ./txts/MensagemCodificada -c ./txts/INArquivoDeChaves -o ./txts/MensagemDecodificada
            livro de cifra :    ./beale -d -i MensagemCodificada -b LivroCifra -o MensagemDecodificada
                                ./beale -d -i ./txts/MensagemCodificada -b ./txts/LivroCifra -o ./txts/MensagemDecodificada
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

int encryptMsg(entryInfo_t *inInfo)
{
    listLetters_t *cipherBookList = NULL;

    printf("Encrypting...\n");

    /* 1. Ler o livro de cifra e armazenar na lista; */
    cipherBookList = cipherBookToList(inInfo->cipherBookPath);

    /* 2. Ler a mensagem original, criptografa-lá e escrever ela no arquivo de saida; */
    encryptAndWrite(inInfo, cipherBookList);

    /* 3. Se pedir, escrever o arquivo de chaves; */
    if (inInfo->encrypting_KeysFile){
        bookToKeysFile(inInfo->keysFilePath, cipherBookList);
    }
}

int decryptMsgWithKeysFile(entryInfo_t *inInfo)
{
    printf("Decrypting with keys file...\n");

    listLetters_t *cipherBookList = NULL;

    /* 1. Ler o arquivo de chaves e armazenar na lista; */
    cipherBookList = keysFileToList(inInfo->keysFilePath);

    /* 2. Ler a mensagem codificada, descriptografa-lá e escrever ela no arquivo de saida; */
    decryptAndWrite(inInfo, cipherBookList);
}

int decryptMsgWithCipherBook(entryInfo_t *inInfo)
{
    printf("Decrypting with cipher book...\n");

    listLetters_t *cipherBookList = NULL;

    /* 1. Ler o livro de cifra e armazenar na lista; */
    cipherBookList = cipherBookToList(inInfo->cipherBookPath);

    /* 2. Ler a mensagem codificada, descriptografa-lá e escrever ela no arquivo de saida; */
    decryptAndWrite(inInfo, cipherBookList);
}

