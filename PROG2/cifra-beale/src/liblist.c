#include <stdio.h>
#include <stdlib.h>

#include "liblist.h"

/* Cria a lista de letras */
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

/* Liberar cada nodo da lista de posicoes da cada nodo da lista de letras */
int destroyList(listLetters_t *list)
{
    nodeLetter_t *aux = list->head;
    nodePosition_t *auxPos = NULL;

    while (aux != NULL){
        auxPos = aux->positions;
        while (auxPos != NULL){
            aux->positions = aux->positions->next;
            free(auxPos);
            auxPos = aux->positions;
        }
        list->head = list->head->next;
        free(aux);
        aux = list->head;
    }

    free(list);

    return 0;
}

/* Insere uma letra na lista */
nodeLetter_t *insertLetter(char letter, listLetters_t *list)
{
    nodeLetter_t *node = (nodeLetter_t *)malloc(sizeof(nodeLetter_t));
    if (! node){
        printf("Falha ao alocar node de letra.\n");
        return NULL;
    }

    node->letter = letter;
    node->repetitions = 0;
    node->positions = NULL;
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

/* Insere uma posição em um nodo contido na lista de letras */
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

/* Procura uma letra na lista e retorna o nodo que a contem */
nodeLetter_t *searchLetter(char letter, listLetters_t *list)
{
    nodeLetter_t *aux = list->head;
    while (aux != NULL){
        if (aux->letter == letter)
            return aux;
        aux = aux->next;
    }
    return NULL;
}

/* Procura uma posição na lista de uma letra e retorna o nodo que a contem */
nodeLetter_t *searchPosition(int position, listLetters_t *list)
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

/* Pega a posição com maior valor númerico da lista */
int getLastPositionFromList(listLetters_t *list)
{
    int max = -10;
    nodeLetter_t *current = list->head;

    while (current->positions == NULL)
        current = current->next;

    /* Percorre a lista principal */
    while (current != NULL) {
        nodePosition_t *currentPos = current->positions;

        /* Percorre a sub-lista encadeada */
        while (currentPos != NULL) {
            if (currentPos->position > max) {
                max = currentPos->position;
            }
            currentPos = currentPos->next;
        }
        
        current = current->next;
    }
    return max;
}

/* Imprime a lista inteira com as repetições de cada letra */
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

/* Imprime a lista com as posiões do menor ao maior */
void reversePrint(nodePosition_t *aux, FILE *file)
{
    if (aux->next != NULL){
        reversePrint(aux->next, file);
    }
    fprintf(file, "%d ", aux->position);
}

/* Imprime a lista direto para um arquivo */
int printListToFile(listLetters_t *list, char *filePath)
{
    FILE *file;

    file = fopen(filePath, "w");
    if (! file){
        printf("Falha ao abrir arquivo de saída.\n");
        return 1;
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

    fclose(file);

    return 0;
}

/* Adicionar nodos de 1 a 9 e de a a z na lista */
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