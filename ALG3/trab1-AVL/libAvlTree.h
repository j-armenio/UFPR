#ifndef _LIB_AVL_TREE_
#define _LIB_AVL_TREE_

typedef struct No {
    int chave;  
    int altura; // Número de ponteiros até o nodo
    struct No *pai;
    struct No *esq;
    struct No *dir;
} No_t;

No_t *inclusao(No_t *raiz, int chave);
No_t *rotEsq(No_t *raiz);
void emOrdem(No_t *raiz);
void atualizaAlt(No_t *raiz);


#endif