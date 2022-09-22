#include <stdio.h>
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
    /* novo->prev = */

    return novo;
}

fila_t *cria_fila()
{
    /* ponteiro para inicio da area da fila */
    fila_t *f;

    /* aloca e verifica se deu certo */
    if (!(f = malloc(sizeof(fila_t))))
        return NULL;

    f->ini = NULL;
    f->fim = NULL;
    f->tamanho = 0;
    return f;
}

fila_t *destroi_fila(fila_t *f)
{
    struct nodo_f *aux;
    /* enquanto a fila nao esta vazia remove os nodos */
    while (!(fila_vazia(f)))
    {
        aux = f->ini;
        f->ini = f->ini->prox;
        free(aux);
    }
    free(f);
    return NULL;
}

int fila_vazia(fila_t *f)
{
    if (f->ini == NULL)
        return 1;
    return 0;
}

int tamanho_fila(fila_t *f)
{
    return f->tamanho;
}

int insere_fila(fila_t *f, int elemento)
{
    nodo_f_t *novo = cria_nodo(elemento);

    /* caso para fila vazia */
    if (fila_vazia(f))
        f->ini = f->fim = novo;

    f->fim->prox = novo;
    f->fim = novo;
    f->tamanho++;
    return 1;
}

int retira_fila(fila_t *f, int *elemento)
{
    /* caso para fila vazia */
    if (fila_vazia(f))
        return 0;

    *elemento = f->ini->chave;
    nodo_f_t *aux = f->ini;
    f->ini = f->ini->prox;
    f->tamanho--;
    free(aux);
    return 1;
}