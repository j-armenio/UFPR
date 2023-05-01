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

/* Recebe as flags de entrada, codifica a mensagem e a imprime no output */
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
    int randomNum, lastPos, i, j;
    nodeLetter_t *letterAux = NULL;
    nodePosition_t *positionAux = NULL;

    char *newLetters;
    newLetters = malloc(sizeof(char) * ARRAY_SIZE);
    if (!newLetters){
        printf("Erro ao alocar matriz.\n");
        return;
    }
    memset(newLetters, 0, ARRAY_SIZE);

    i = 0;

    while ((c = fgetc(originalMsg)) != EOF)
    {
        if (isalnum(c))
        {

            letterAux = searchLetter(tolower(c), cipherBookList);
            positionAux = letterAux->positions;

            if (positionAux == NULL)
            { /* iteracao que ocorre toda vez que uma letra que nao estava registrada aparece */
                lastPos = getLastPositionFromList(cipherBookList);
                insertPosition(lastPos+1, letterAux);
                
                newLetters[i] = letterAux->letter;
                i++;

                fprintf(outputMsg, "%d ", lastPos+1);
            }
            else
            {
                for (j = 0; j <= i; j++){
                    if (letterAux->letter == newLetters[j]){
                        lastPos = getLastPositionFromList(cipherBookList);
                        insertPosition(lastPos+1, letterAux);
                    }
                }

                randomNum = getRandomNumber(0, letterAux->repetitions);

                while (randomNum > 0 && positionAux->next != NULL)
                {
                    positionAux = positionAux->next;
                    randomNum--;
                }
            
                fprintf(outputMsg, "%d ", positionAux->position);
            }

        } else if (c == ' '){
            fprintf(outputMsg, "-1 ");
        } else if (c == '\n'){
            fprintf(outputMsg, "-2 ");
        }
    }

    free(newLetters);
    fclose(originalMsg);
    fclose(outputMsg);
}

int encryptMsg(entryInfo_t *inInfo)
{
    listLetters_t *cipherBookList = NULL;

    /* 1. Ler o livro de cifra e armazenar na lista; */
    cipherBookList = cipherBookToList(inInfo->cipherBookPath);

    /* 2. Se pedir, escrever o arquivo de chaves; */
    if (inInfo->encrypting_KeysFile){
        bookToKeysFile(inInfo->keysFilePath, cipherBookList);
    }

    /* 3. Ler a mensagem original, criptografa-lá e escrever ela no arquivo de saida; */
    encryptAndWrite(inInfo, cipherBookList);

    /* 4. Liberar a lista; */
    destroyList(cipherBookList);

    return 0;
}

/* ------------------ DECRYPT FUNCTIONS ------------------ */

/* Recebe as flags de entrada, decodifica a mensagem e a imprime no output */
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

    char line[2048];
    char *token;

    while (! feof (originalMsg))
    {
        fgets(line, 2048, originalMsg);
        if (line[0] == '\0')
            break;
        if (line[0] == ' ')
            continue;
        token = strtok(line, " ");

        while (token != NULL){
            if (atoi(token) == -1){
                fprintf(outputMsg, " ");
            } else if (atoi(token) == -2){
                fprintf(outputMsg, "\n");
            } else {
                if (searchPosition(atoi(token), cipherBookList) == NULL){ /* letra nao pertencente ao registro */
                    fprintf(outputMsg, "X");
                } else {
                    fprintf(outputMsg, "%c", searchPosition(atoi(token), cipherBookList)->letter);
                }
            }
            token = strtok(NULL, " ");
        } 
    }

    fclose(originalMsg);
    fclose(outputMsg);
}

int decryptMsgWithKeysFile(entryInfo_t *inInfo)
{
    listLetters_t *cipherBookList = NULL;

    /* 1. Ler o arquivo de chaves e armazenar na lista; */
    cipherBookList = keysFileToList(inInfo->keysFilePath);

    /* 2. Ler a mensagem codificada, descriptografa-lá e escrever ela no arquivo de saida; */
    decryptAndWrite(inInfo, cipherBookList);

    /* 3. Liberar a lista; */
    destroyList(cipherBookList);

    return 0;
}

int decryptMsgWithCipherBook(entryInfo_t *inInfo)
{
    listLetters_t *cipherBookList = NULL;

    /* 1. Ler o livro de cifra e armazenar na lista; */
    cipherBookList = cipherBookToList(inInfo->cipherBookPath);

    /* 2. Ler a mensagem codificada, descriptografa-lá e escrever ela no arquivo de saida; */
    decryptAndWrite(inInfo, cipherBookList);

    /* 3. Liberar a lista; */
    destroyList(cipherBookList);

    return 0;
}