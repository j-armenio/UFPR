#include <stdio.h>
#include "bstlib.h"

int main()
{
    No_t *raiz = inicializa_bst();
    
    raiz = inclusao(raiz, 30);
    raiz = inclusao(raiz, 15);
    raiz = inclusao(raiz, 50);
    raiz = inclusao(raiz, 25);
    raiz = inclusao(raiz, 10);
    raiz = inclusao(raiz, 31);

    imprime_emOrdem(raiz);

    libera_arvore(raiz);

    return 0;
}