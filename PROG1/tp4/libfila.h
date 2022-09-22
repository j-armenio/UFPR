#ifndef _LIBfila_t_H
#define _LIBfila_t_H

struct fila {
    int *elementos;       /* espaco armazenamento dos elementos  */
    int tamanho_max;      /* numero maximo de elementos na lista */
    int cabeca;           /* ponteiro para a cabeca da lista     */
    int cauda;            /* ponteiro para a cauda da lista      */
};
typedef struct fila fila_t;

/*
 * Cria uma fila vazia de tamanho maximo max_elementos e retorna um ponteiro para ela, se falhar retorna NULL.
*/
fila_t* fila_cria (int max_elementos);

/*
 * Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
*/
int queue (fila_t* f, int elemento);

/*
 * Remove elemento do inicio da fila (politica FIFO) e o retorna em "elemento".
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
*/
int dequeue (fila_t* f, int* elemento);

/*
 * Retorna o numero de elementos validos presentes na fila
*/
int fila_tamanho (fila_t* f);

/*
 * Retorna 1 se a fila esta vazia e 0 caso contrario.
*/
int fila_vazia (fila_t* f);

/*
 * Remove todos os elementos da fila, libera espaco e devolve NULL.
*/
fila_t* fila_destroi (fila_t* f);

#endif
