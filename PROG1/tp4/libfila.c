#include <stdlib.h>
#include "libfila.h"

int fila_cheia(fila_t* f);

/* */

fila_t *fila_cria (int max_elementos)
{   
    /* cria fila */
    fila_t *f;
    if (!(f = malloc(sizeof(int))))
        return NULL;

    if (!(f->elementos = malloc(sizeof(int) * max_elementos))){
        free(f);
        return NULL;
    }

    f->tamanho_max = max_elementos;
    f->cabeca = 0;
    f->cauda = 0;

    return f;
}

int queue (fila_t* f, int elemento)
{
    if (fila_cheia(f))
        return 0;

    f->elementos[f->cauda] = elemento;
    f->cauda = (f->cauda + 1) % f->tamanho_max;
    return 1;
}

int dequeue (fila_t* f, int* elemento)
{
    if (fila_vazia(f))
        return 0;
    
    elemento = &f->elementos[f->cabeca];
    f->cabeca = (f->cabeca + 1) % f->tamanho_max;
    return 1;
}

int fila_tamanho (fila_t* f)
{
    int tamanho;
    int cabeca_temp;

    if (fila_vazia(f)){
        return 0;
    } 
    else if (f->cabeca < f->cauda){
        tamanho = f->elementos[f->cauda] - f->elementos[f->cabeca];
    }
    else{ /* cabeca > cauda */
        cabeca_temp = f->cabeca;
        tamanho = 0;

        while (cabeca_temp < f->cauda){
            cabeca_temp = (cabeca_temp + 1) % f->tamanho_max;
            tamanho++;
        }
    }

    return tamanho;
}

int fila_vazia (fila_t* f)
{
    /* fila vazia retorna 1 */
    if (f->cauda == f->cabeca)
        return 1;

    return 0;
}   

int fila_cheia(fila_t* f)
{
    /* se fila cheia retorna 1 */
    if (((f->cauda + 1) % f->tamanho_max) == f->cabeca)
        return 1;

    return 0;
}

fila_t* fila_destroi (fila_t* f)
{
    free(f->elementos);
    free(f);
    return NULL;
}
