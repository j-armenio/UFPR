#include <stdio.h>
#include <stdlib.h>
#include "libAvlTree.h"

// Retorna um ponteiro para um novo no
No_t *criaNo(int chave)
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

    return no;
}

// Busca uma chave na arvore e caso ele exista, retorna seu no
No_t *buscaBin(No_t *raiz, int chave)
{
    if (raiz == NULL)
        return NULL;

    if (raiz->chave == chave)
        return raiz;

    if (chave < raiz->chave)
        return buscaBin(raiz->esq, chave);
    else
        return buscaBin(raiz->dir, chave);
}

// Calcula a altura de qualquer no da arvore
int alturaNo(No_t *no)
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

// Calcula a profundidade de qualquer no da arvore, por travessia pre-ordem
void profundidadeNo(No_t *no, int nivel)
{
    if (no == NULL)
        return;

    no -> nivel = nivel; 
    nivel++;
    profundidadeNo(no->esq, nivel);
    profundidadeNo(no->dir, nivel);
}

// Calcula o Fator de Balanceamento de um no qualquer da arvore
int calculaFb(No_t *no)
{
    if (no->esq == NULL && no->dir == NULL)
        return 0;

    return alturaNo(no->esq) - alturaNo(no->dir);}

// Atualiza a altura de todos nos da arvore
void atualizaAlt(No_t *raiz)
{
    if (raiz == NULL)
        return;
    
    atualizaAlt(raiz->esq);
    raiz->altura = alturaNo(raiz);
    atualizaAlt(raiz->dir);
}

// Realiza a rotação para esquerda e retorna a nova raiz
No_t *rotEsq(No_t *x)
{
    No_t *y = x->dir;
    No_t* T2 = y->esq;
  
    // Perform rotation
    y->esq = x;
    x->dir = T2;
  
    //  Update heights
    atualizaAlt(x);
    atualizaAlt(y);
  
    // Return new root
    return y;
}

// Realiza a rotação para direita e retorna a nova raiz
No_t *rotDir(No_t *y)
{
    No_t *x = y->esq;
    No_t *T2 = x->dir;
  
    // Executa rotação
    x->dir = y;
    y->esq = T2;
  
    // Atualiza altura da arvore
    atualizaAlt(y);
    atualizaAlt(x);

    return x;
}

No_t *balanceiaAvl(No_t* raiz, int chave)
{
    No_t* noBal;
    if(calculaFb(raiz) > 1){
        if(calculaFb(raiz->esq) < 0)
            raiz->esq = rotEsq(raiz->esq);
        noBal = rotDir(raiz);
    }
    else if(calculaFb(raiz) < -1){
        if(calculaFb(raiz->dir) > 0)
            raiz->dir = rotDir(raiz->dir);
        noBal = rotEsq(raiz);
    }

    atualizaAlt(raiz);

    return noBal;
}

// Inclui um nodo com a chave inserida e retorna a raiz
No_t *inclusao(No_t *raiz, int chave)
{
    No_t* noBal;

    // Critério de parada: AVL vazia ou chegou a NULL
    if (raiz == NULL){
        raiz = criaNo(chave);
        return raiz;
    }
    // Encontra a posição da chave na árvore e adiciona o nodo
    if (chave > raiz->chave){
        raiz->dir = inclusao(raiz->dir, chave);
    }
    else if (chave < raiz->chave){
        raiz->esq = inclusao(raiz->esq, chave);
    } 
    else {
        return raiz;
    }

    atualizaAlt(raiz);

    raiz = balanceiaAvl(raiz, chave);

    /* return the (unchanged) raiz pointer */
    return raiz;
}

// Remove um nodo com a chave inserida e retorna a raiz
No_t *remocao(No_t *raiz, int chave)
{
    No_t *aux;

    if (raiz == NULL)
        return NULL;
    
    if (raiz->chave == chave){
        // Caso 1: no folha (sem filhos)
        if ((raiz->esq == NULL) && (raiz->dir == NULL)){
            free(raiz);
            return NULL;
        } else {
            aux = raiz->esq;
            // Caso 2: no com 2 filhos
            if ( (raiz->esq != NULL) && (raiz->dir != NULL) ){
                while (aux->dir != NULL)
                    aux = aux->dir;
                raiz->chave = aux->chave;
                aux->chave = chave;
                raiz->esq = remocao(raiz->esq, chave);
                return raiz;
            } else {
                // Caso 3: no com 1 filho
                if (raiz->esq != NULL)
                    aux = raiz->esq;
                else
                    aux = raiz->dir;
                free(raiz);
                return aux;
            }
        }
    } else {
        if (chave < raiz->chave)
            raiz->esq = remocao(raiz->esq, chave);
        else
            raiz->dir = remocao(raiz->dir, chave);
    }

    atualizaAlt(raiz);

    raiz = balanceiaAvl(raiz, chave);

    return raiz;
}


// Travesia emOrdem(esq->raiz->dir)
void emOrdem(No_t *raiz)
{
    if (raiz == NULL)
        return;
    
    emOrdem(raiz->esq);
    printf("%d,%d\n", raiz->chave, raiz->nivel);
    emOrdem(raiz->dir);
}

// Libera a memoria de toda arvore
void destroiArvore(No_t *raiz)
{
    if (raiz == NULL)
        return;
    destroiArvore(raiz->esq);
    destroiArvore(raiz->dir);
    free(raiz);
}
