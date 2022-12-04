#include <stdio.h>
#include <stdlib.h>
#include "libAvlTree.h"

int main()
{
    No_t *raiz = NULL;

    raiz = inclusao(raiz, 10);
    raiz = inclusao(raiz, 20);
    raiz = inclusao(raiz, 30);

    rotEsq(raiz);
    
    atualizaAlt(raiz);

    emOrdem(raiz);
}