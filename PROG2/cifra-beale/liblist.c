#include <stdio.h>
#include <stdlib.h>

#include "liblist.h"

listLetters_t *createLettersList()
{
    listLetters_t *list = (listLetters_t *)malloc(sizeof(listLetters_t));
    if (! list){
        printf("Falha ao alocar lista de letras.\n");
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

nodeLetter_t *insertLetter(char letter, listLetters_t *list)
{
    nodeLetter_t *node = (nodeLetter_t *)malloc(sizeof(nodeLetter_t));
    if (! node){
        printf("Falha ao alocar node de letra.\n");
        return NULL;
    }

    node->letter = letter;
    node->repetitions = 0;
    node->next = NULL;

    if (list->size == 0){
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;

    return node;
}

nodePosition_t *insertPosition(int position, nodeLetter_t *node)
{
    nodePosition_t *nodePos = (nodePosition_t *)malloc(sizeof(nodePosition_t));
    if (! nodePos){
        printf("Falha ao alocar node de posição.\n");
        return NULL;
    }

    nodePos->position = position;
    nodePos->next = NULL;

    if (node->positions == NULL){
        node->positions = nodePos;
    } else {
        nodePosition_t *aux = node->positions;
        while (aux->next != NULL){
            aux = aux->next;
        }
        aux->next = nodePos;
    }

    node->repetitions++;

    return nodePos;
}

nodeLetter_t *searchLetter(char letter, listLetters_t *list)
{
    nodeLetter_t *aux = list->head;
    while (aux != NULL){
        if (aux->letter == letter){
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

nodeLetter_t *searchPosition(char position, listLetters_t *list)
{
    nodeLetter_t *aux = list->head;
    nodePosition_t *auxPos = NULL;

    while (aux != NULL){
        auxPos = aux->positions;
        while (auxPos != NULL){
            if (auxPos->position == position){
                return aux;
            }
            auxPos = auxPos->next;
        }
        aux = aux->next;
    }
    return NULL;
}

void printList(listLetters_t *list)
{
    nodeLetter_t *aux = list->head;

    while (aux != NULL){
        printf("%c: %d: ", aux->letter, aux->repetitions);

        nodePosition_t *auxPos = aux->positions;
        while (auxPos != NULL){
            printf("%d ", auxPos->position);
            auxPos = auxPos->next;
        }
        printf("\n");
        aux = aux->next;
    }
}

void reversePrint(nodePosition_t *aux, FILE *file)
{
    if (aux->next != NULL){
        reversePrint(aux->next, file);
    }
    fprintf(file, "%d ", aux->position);
}

void printListToFile(listLetters_t *list, char *filePath)
{
    FILE *file;

    file = fopen(filePath, "w");
    if (! file){
        printf("Falha ao abrir arquivo de saída.\n");
        return;
    }

    nodeLetter_t *aux = list->head;

    while (aux != NULL){
        if (aux->positions != NULL){
            fprintf(file, "%c: ", aux->letter);
            reversePrint(aux->positions, file);
            fprintf(file, "\n");
        }
        aux = aux->next;
    }
}

// Adicionar nodos de 1 a 9 e de a a z
listLetters_t *indexAllList(listLetters_t *list)
{
    int i;
    char letter;

    for (i = 0; i < 10; i++)
        insertLetter(i + '0', list);

    for (letter = 'a'; letter <= 'z'; letter++)
        insertLetter(letter, list);

    return list;
}