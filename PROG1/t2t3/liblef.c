#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblef.h"
#include "libaleat.h"

nodo_lef_t *cria_nodo_evento(evento_t *evento)
{
    nodo_lef_t *novo;

    /* aloca espaco para o nodo */
    if (! (novo = malloc(sizeof(nodo_lef_t))))
        return NULL;

    /* aloca espaco para o evento antes de copiar do main */
    if (! (novo->evento = malloc(sizeof(evento_t))))
        return NULL;

    /* aloca espaco para os dados do evento */
    if (! (novo->evento->dados = malloc(novo->evento->tamanho)))
        return NULL;

    /* memcpy(novo->evento, evento, sizeof(evento_t)); */

    /* preenche os dados do evento evento */
    memcpy(novo->evento->dados, evento->dados, evento->tamanho);
    novo->evento->destroidados = evento->destroidados;
    novo->evento->tamanho = evento->tamanho;
    novo->evento->tempo = evento->tempo;
    novo->evento->tipo = evento->tipo;

    novo->prox = NULL;

    return novo;
}

lef_t *cria_lef ()
{
    lef_t *l;

    if (! (l = malloc(sizeof(nodo_lef_t))))
        return NULL;

    nodo_lef_t *n;

    if (! (n = malloc(sizeof(nodo_lef_t))))
        return NULL;
    if (! (n->evento = malloc(sizeof(evento_t))))
        return NULL;

    n->evento = NULL;
    n->prox = NULL;
    l->Primeiro = n;

    return l;
}

lef_t * destroi_lef (lef_t *l)
{
    return NULL;
}

int adiciona_inicio_lef (lef_t *l, evento_t *evento)
{
    nodo_lef_t *novo = cria_nodo_evento(evento);

    novo->prox = l->Primeiro->prox;
    l->Primeiro->prox = novo;

    return 1;
}

int adiciona_ordem_lef  (lef_t *l, evento_t *evento)
{
    nodo_lef_t *novo = cria_nodo_evento(evento);

    /* se fila vazia, soh adicionar no inicio */
    if (l->Primeiro->prox == NULL){
        adiciona_inicio_lef(l, evento);
        return 1;
    }
    /* se novo->tempo < head->tempo, fazer dele a cabeca */
    if (novo->evento->tempo < l->Primeiro->prox->evento->tempo){
        novo->prox = l->Primeiro->prox;
        l->Primeiro->prox = novo;
        return 1;
    } else {
    /* faca um loop, comecando da cabeca e va movendo ate achar
    um nodo cujo n->tempo > novo->tempo.
    O nodo logo antes de n eh a posicao apropriada.
    Inserir o novo na posicao de n */
        nodo_lef_t *aux = l->Primeiro->prox;
        while ((aux->prox != NULL) && (aux->prox->evento->tempo < novo->evento->tempo))
            aux = aux->prox;
        novo->prox = aux->prox;
        aux->prox = novo;
        return 1;
    }
}

evento_t * obtem_primeiro_lef (lef_t *l)
{
    if (l->Primeiro->prox != NULL){
        nodo_lef_t *aux = cria_nodo_evento(l->Primeiro->prox->evento);
        l->Primeiro->prox = l->Primeiro->prox->prox;
        return aux->evento;
    }
    else{ /* lista vazia */
        return NULL;
    }
}
