#ifndef _LIBfila_t_H
#define _LIBfila_t_H

struct nodo_f {
    int chave;            /* fila de numeros inteiros */
    struct nodo_f *prox;  /* ponteiro para o proximo  */
};
typedef struct nodo_f nodo_f_t;

struct fila {
    nodo_f_t* cabeca;     /* ponteiro para o inicio da fila */
    nodo_f_t* cauda;      /* ponteiro para o fim da fila    */
    int tamanho;          /* numero de elementos na fila    */
};
typedef struct fila fila_t;

/*
 * Cria uma fila vazia de tamanho maximo max_elementos e retorna um ponteiro para ela, se falhar retorna NULL.
*/
fila_t* fila_cria ();

/*
 * Insere o elemento no final da fila (politica FIFO).
 * Retorna 1 se a operacao foi bem sucedida e 0 caso contrario.
*/
int queue (fila_t* f, int elemento);

/*
 * Remove elemento do inicio da fila (politica FIFO) e o devolve atraves
 * do parametro "*elemento". Nao confundir com o retorno da funcao.
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
