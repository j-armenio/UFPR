#include <stdio.h>
#include <stdlib.h>
#include "libAvlTree.h"

No_t *criaNo(int chave) // OOOKK
{
    No_t *no = malloc(sizeof(No_t));
    if (! no){
        perror("Falha ao alocar no\n");
        return NULL;
    }

    no->chave = chave;
    no->altura = 0;
    no->dir = NULL;
    no->esq = NULL;
    no->pai = NULL;

    return no;
}

int alturaNo(No_t *no) // OOOKK
{
    int altEsq = 0;
    int altDir = 0;

    if (no == NULL)
        return -1;

    altEsq = alturaNo(no->esq);
    altDir = alturaNo(no->dir);

    if (altEsq > altDir)
        return altEsq + 1;
    else
        return altDir + 1;
}

int calculaFb(No_t *no)
{
    return alturaNo(no->esq) - alturaNo(no->dir);
}

void atualizaAlt(No_t *raiz) // OOOKK
{
    if (raiz == NULL)
        return;
    
    atualizaAlt(raiz->esq);
    raiz->altura = alturaNo(raiz);
    atualizaAlt(raiz->dir);
}

No_t *rotEsq(No_t *raiz)
{
    No_t *fDir = raiz->dir;

    raiz->dir = fDir->esq;
    fDir->pai = raiz->pai;
    raiz->pai = fDir;
    if (fDir->esq != NULL)
        fDir->esq->pai = raiz;
    fDir->esq = raiz;

    return fDir;
}

No_t *rotDir(No_t *raiz)
{

}

No_t *balanceiaAvl()
{


    return NULL;
}

No_t *inclusao(No_t *raiz, int chave)
{
    // Critério de parada: AVL vazia ou chegou a NULL
    if (raiz == NULL){
        raiz = criaNo(chave);
        return raiz;
    }

    // Encontra a posição da chave na árvore e adiciona o nodo
    if (chave > raiz->chave){
        raiz->dir = inclusao(raiz->dir, chave);
        raiz->dir->pai = raiz;

    } else {
        raiz->esq = inclusao(raiz->esq, chave);
        raiz->esq->pai = raiz;
    }

    // Atualiza a altura de todos nos da AVL
    atualizaAlt(raiz);

    // Balanceia a AVL
    // raiz = balanceiaAvl(raiz);

    return raiz;
}

void emOrdem(No_t *raiz)
{
    if (raiz == NULL)
        return;
    
    emOrdem(raiz->esq);
    printf("chave:%d alt:%d\n", raiz->chave, alturaNo(raiz));
    emOrdem(raiz->dir);
}