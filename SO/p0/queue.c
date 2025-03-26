/*
Nome: João Armênio Silveira

GRR: 20211792
*/

#include <stdio.h>
#include "queue.h"

int queue_size(queue_t *queue) 
{
    // Caso fila vazia
    if (queue == NULL)
        return 0;
    
    queue_t *aux = queue;
    int n=0;

    // Percorre lista
    do {
        n++;
        aux = aux->next;
    } while (aux != queue);

    return n;
}

void queue_print(char *name, queue_t *queue, void print_elem (void*)) 
{
    if (queue == NULL){
        printf("[]\n");
        return;
    }

    printf("%s: [", name);

    queue_t *aux = queue;

    do {
        print_elem(aux);
        aux = aux->next;
        if (aux != queue)
            printf(" "); // separador
    } while (aux != queue);

    printf("]\n");
}

int queue_append(queue_t **queue, queue_t *elem) {
    // fila deve existir
    if (!queue) {
        fprintf(stderr, "###Erro: Fila não existe.\n");
        return -1;
    }
    // elemento deve existir
    if (!elem) {
        fprintf(stderr, "###Erro: Elemento a ser adicionado não existe.\n");
        return -1;
    }
    // elemento não deve estar em outra fila
    if (elem->next || elem->prev) {
        fprintf(stderr, "###Erro: Elemento pertence a outra fila.\n");
        return -1;
    }

    // Fila vazia, novo elemento será o primeiro
    if (*queue == NULL) {
        *queue = elem;
        elem->next = elem;
        elem->prev = elem;
    } else { 
        // Insere no final da fila
        queue_t *last = (*queue)->prev;
        last->next = elem;
        elem->prev = last;
        elem->next = *queue;
        (*queue)->prev = elem;
    }
    return 0;
}

int queue_remove(queue_t **queue, queue_t *elem) {
    // fila deve existir
    if (!queue) {
        fprintf(stderr, "###Erro: Fila não existe.\n");
        return -1;
    }
    // a fila não deve estar vazia
    if (!*queue){
        fprintf(stderr, "#Erro: Fila está vazia.\n");
        return -1;
    }
    // elemento deve existir
    if (!elem) {
        fprintf(stderr, "###Erro: Elemento a ser removido não existe.\n");
        return -1;
    }
    // elemento deve pertencer a fila indicada
    queue_t *aux = *queue;
    do {
        if (aux == elem)
            break;
        aux = aux->next;
    } while (aux != *queue);
    if (aux != elem) {
        fprintf(stderr, "###Erro: Elemento não pertence a fila.\n");
        return -1;
    }

    // Caso fila com 1 elemento
    if (elem->next == elem) {
        *queue = NULL;
    } else {
        // remoção do primeiro elemento
        if (elem == *queue)
            *queue = elem->next;
        
        elem->prev->next = elem->next;
        elem->next->prev = elem->prev;
    }

    elem->next = NULL;
    elem->prev = NULL;

    return 0;
}
