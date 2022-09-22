#include <stdlib.h>
#include "libpilha.h"

PILHA *piCria(int nelem)
{
    if (nelem <= 0) return NULL;

    /* cria uma pilha */
    PILHA *pilha;
    if (!(pilha = malloc(sizeof(PILHA)))) return NULL;

    /* aloca memoria para o array */
    pilha->p_elems = malloc(sizeof(char) * nelem);
    if (pilha->p_elems == NULL){
        free(pilha);
        return NULL;
    }

    pilha->p_nelem = nelem;
    pilha->p_topo = 0;

    return pilha;
}

int piPush(PILHA *pilha, char elem)
{
    /*retorna -1 caso pilha esteja cheia*/
    if (pilha->p_topo == pilha->p_nelem) return -1;
    
    pilha->p_topo++;
    pilha->p_elems[pilha->p_topo] = elem;

    return pilha->p_topo;
}

char piPop(PILHA *pilha)
{
    int aux;

    if (piVazia(pilha)) return 0;
    aux = pilha->p_elems[pilha->p_topo];
    pilha->p_elems[pilha->p_topo] = 0;

    pilha->p_topo--;

    return aux;
}

char piTop(PILHA *pilha)
{
    /*verifica se a pilha está vazia */
    if (piVazia(pilha)) return 0;
    return pilha->p_topo;
}

int piVazia(PILHA *pilha)
{
    if (pilha->p_topo == 0) return 1;
    return 0;
}

void piDestroi(PILHA *pilha)
{
    free(pilha->p_elems);
    free(pilha);
}