#include <stdio.h>
#include <stdlib.h>
#include "bstlib.h"

// ############# FUÇÕES INTERNAS #############

No_t *cria_no(int chave)
{
    No_t *no = (No_t *)malloc(sizeof(No_t));
    no->chave = chave;
    no->dir = no->esq = NULL;
    return no;
}

// ############# FUÇÕES DA LIB #############

// Inicializa uma árvore vazia, retorna um nodo NULL
No_t *inicializa_bst()
{
    return NULL;
}

// Passa a raiz e o item que você quer colocar. 
No_t *inclusao(No_t *raiz, int chave)
{
    No_t *aux;

    if (raiz == NULL){ // Critério de parada. Se NAO ha um no raiz
        aux = cria_no(chave);
        return aux;
    } else {           // Se ha um no raiz
        if (chave < raiz->chave)  // insere na subarvore da esquerda
            raiz->esq = inclusao(raiz->esq, chave);
        else                      // insere na subarvore da direita
            raiz->dir = inclusao(raiz->dir, chave);
    }
    return raiz;
}

// esq->raiz->dir
void imprime_emOrdem(No_t *raiz)
{
    if (raiz != NULL){
        imprime_emOrdem(raiz->esq);
        printf("%d ", raiz->chave);
        imprime_emOrdem(raiz->dir);
    }
}

void libera_arvore(No_t *raiz)
{
    if (raiz != NULL){
        libera_arvore(raiz->esq);
        libera_arvore(raiz->dir);
        free(raiz);
    }
}