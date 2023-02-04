#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libAvlTree.h"

#define MAX 1024

int main()
{
    No_t *raiz = NULL;
    No_t *nodo = NULL;

    int nivel =0;

    int comando, valor;

    fscanf(stdin, "%lc %d", &comando, &valor);
    getc(stdin);
    
    while (! feof(stdin)){
        if (comando == 'i')
            raiz = inclusao(raiz, valor);
        else if ((nodo = buscaBin(raiz, valor)))
            raiz = remocao(raiz, valor);

        

        fscanf(stdin, "%lc %d", &comando, &valor);
        getc(stdin);
    }

    atualizaAlt(raiz);
    
    profundidadeNo(raiz, nivel);

    emOrdem(raiz);

    destroiArvore(raiz);

    return 0;
}
