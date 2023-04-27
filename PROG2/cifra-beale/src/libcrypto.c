#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libcrypto.h"

/* ------------------ PR0TOTYPES ------------------ */

void encryptAndWrite(entryInfo_t *inInfo, listLetters_t* cipherBookList);
int encryptMsg(entryInfo_t *inInfo);

void decryptAndWrite(entryInfo_t *inInfo, listLetters_t* cipherBookList);
int decryptMsgWithKeysFile(entryInfo_t *inInfo);
int decryptMsgWithCipherBook(entryInfo_t *inInfo);

/* ------------------ ENCRYPT FUNCTIONS ------------------ */

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
    int randomNum, lastPos;
    nodeLetter_t *letterAux = NULL;
    nodePosition_t *positionAux = NULL;

    printList(cipherBookList);

    while ((c = fgetc(originalMsg)) != EOF)
    {
        if (isalnum(c))
        {
            letterAux = searchLetter(tolower(c), cipherBookList);
            positionAux = letterAux->positions;

            if (positionAux == NULL) /* eh uma letra que nao tem nenhuma posicao, não esta na referencia */
            {
                /*  preciso pegar a maior posicao da lista inteira, a letra que não esta contida na lista e
                    criar um novo nodoPosition para essa letra. */
                lastPos = getLastPositionFromList();
            
            }

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
    return 0;
}

/* ------------------ DECRYPT FUNCTIONS ------------------ */

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

int decryptMsgWithKeysFile(entryInfo_t *inInfo)
{
    printf("Decrypting with keys file...\n");

    listLetters_t *cipherBookList = NULL;

    /* 1. Ler o arquivo de chaves e armazenar na lista; */
    cipherBookList = keysFileToList(inInfo->keysFilePath);

    /* 2. Ler a mensagem codificada, descriptografa-lá e escrever ela no arquivo de saida; */
    decryptAndWrite(inInfo, cipherBookList);

    return 0;
}

int decryptMsgWithCipherBook(entryInfo_t *inInfo)
{
    printf("Decrypting with cipher book...\n");

    listLetters_t *cipherBookList = NULL;

    /* 1. Ler o livro de cifra e armazenar na lista; */
    cipherBookList = cipherBookToList(inInfo->cipherBookPath);

    /* 2. Ler a mensagem codificada, descriptografa-lá e escrever ela no arquivo de saida; */
    decryptAndWrite(inInfo, cipherBookList);

    return 0;
}