#include <stdio.h>
#include <stdlib.h>

struct node {
    int chave;
    struct node *pai, *dir, *esq;
}; 
typedef struct node no;

/* Cria o nodo e retorna ele */
no *cria_nodo(int chave){
    no *n = malloc(sizeof(no));
    n->chave = chave;
    n->pai = n->dir = n->esq = NULL;
    return n;
}

/* Imprime a bst com Travesia emOrdem (esq, raiz, dir) */
void em_ordem(no *raiz){
    if (raiz != NULL){
        /* esq */
        em_ordem(raiz->esq);
        /* raiz */
        printf("%d")
    }

}
