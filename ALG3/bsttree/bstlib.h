#ifndef __BSTLIB__
#define __BSTLIB__
 
struct No {
    int chave;
    struct No *esq;
    struct No *dir;
};
typedef struct No No_t;

No_t *inicializa_bst();
No_t *inclusao(No_t *raiz, int chave);
void imprime_emOrdem(No_t *raiz);
void libera_arvore(No_t *raiz);

#endif