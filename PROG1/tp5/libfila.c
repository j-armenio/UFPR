#include <stdlib.h>
#include "libfila.h"


nodo_f_t *cria_nodo(int chave)
{
    /* ponteiro para inicio da area do nodo */
    nodo_f_t *novo;

    /* verifica se a alocacao deu certo */
    if (!(novo = malloc(sizeof(nodo_f_t))))
        return NULL;
    
    novo->chave = chave;
    novo->prox = NULL;

    return novo;
}

fila_t *fila_cria()
{
    /* ponteiro para inicio da area da fila */
    fila_t *f;

    /* verifica se a alocacao deu certo */
    if (!(f = malloc(sizeof(fila_t))))
        return NULL;
    
    f->cabeca = NULL;
    f->cauda = NULL;
    f->tamanho = 0;
    return f;
}

int queue(fila_t* f, int elemento)
{
    nodo_f_t *novo = cria_nodo(elemento);

    /* caso para fila vazia */
    if (fila_vazia(f))
        f->cabeca = f->cauda = novo;

    f->cauda->prox = novo;
    f->cauda = novo;
    f->tamanho++;
    return 1;
}

int dequeue(fila_t* f, int* elemento)
{
    /* caso para fila vazia */
    if (fila_vazia(f))
        return 0;

    *elemento = f->cabeca->chave;
    nodo_f_t *aux = f->cabeca;
    f->cabeca = f->cabeca->prox;
    f->tamanho--;
    free(aux);
    return 1;
}

int fila_tamanho(fila_t* f)
{
    return f->tamanho;
}

int fila_vazia(fila_t* f)
{
    if (f->cabeca == NULL)
        return 1;
    return 0;
}

fila_t* fila_destroi(fila_t* f)
{
    struct nodo_f *aux;
    /* enquanto a fila nao esta vazia remove os nodos */
    while (!(fila_vazia(f))){
        aux = f->cabeca;
        f->cabeca = f->cabeca->prox;
        free(aux);
    }
    free(f);
    return NULL;
}