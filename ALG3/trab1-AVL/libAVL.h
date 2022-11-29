#ifndef __LIBAVL__
#define __LIBAVL__
 
struct No {
    int chave;
    int altura;
    struct No *esq, *dir, *pai;
};
typedef struct No No_t;
 
#endif
