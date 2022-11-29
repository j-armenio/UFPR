#include <stdio.h>
#include <stdlib.h>
#include "libAVL.h"

/* ################# FUNÇÕES INTERNAS ################# */

/* Aloca o nó e inicializa seus parâmetros*/
No_t *cria_no(int chave)
{
    No_t *no = malloc(sizeof(No_t));

    no->chave = chave;
    no->altura = 0;
    no->esq = no->dir = no->pai = NULL;

    return no;
}

/* Recebe dois inteiros e retorna o maior deles 
 * Uso: a altura de um nó é a maior altura de suas subarvores + 1 */
int maior(int a, int b)
{
    return (a > b)? a: b;
}

/* Retorna a altura de um no, retorna 0 caso nulo */
int altura(No_t *no)
{
    if (no)
        return (no->altura);
    else
        return 0;
}

/* Retorna o fator de balanceamento do no, retorna 0 caso nulo */
int fb_no(No_t *no)
{
    if (no)
        return (altura(no->esq) - altura(no->dir));
    else
        return 0;
}

/* ################# FUNÇÕES LIB ################# */

/* Rotações: 
 * Recebem um nodo raiz de uma árvore/subárvore e retornam o endereço do nó que sera a nova raiz */
No_t *rot_esq(No_t *raiz)
{
    No_t *aux = raiz->dir;

    raiz->dir = aux->esq;
    aux->pai = raiz->pai;
    raiz->pai = aux;
    if (aux->esq != NULL)
        aux->esq->pai = raiz;
    aux->esq = raiz;

    raiz->altura = maior(altura(raiz->esq), altura(raiz->dir)) + 1;
    aux->altura = maior(altura(aux->esq), altura(aux->dir)) + 1;

    return aux;
}
No_t *rot_dir(No_t *raiz)
{
    No_t *aux = raiz->esq;

    raiz->esq = aux->dir;
    aux->pai = raiz->pai;
    raiz->pai = aux;
    if (aux->dir != NULL)
        aux->dir->pai = raiz;
    aux->dir = raiz;

    raiz->altura = maior(altura(raiz->esq), altura(raiz->dir)) + 1;
    aux->altura = maior(altura(aux->esq), altura(aux->dir)) + 1;

    return aux;
}

