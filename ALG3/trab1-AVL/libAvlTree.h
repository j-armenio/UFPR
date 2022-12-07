#ifndef _LIB_AVL_TREE_
#define _LIB_AVL_TREE_

typedef struct No {
    int chave;  
    int altura;
    int nivel;
    struct No *pai;
    struct No *esq;
    struct No *dir;
} No_t;

No_t *inclusao(No_t *raiz, int chave);
No_t *remocao(No_t *raiz, int chave);
No_t *buscaBin(No_t *raiz, int chave);
void profundidadeNo(No_t *no, int nivel);
void atualizaAlt(No_t *raiz);
void emOrdem(No_t *raiz);
void destroiArvore(No_t *raiz);

#endif