/*
 * Testes para a liblef.c
 * Disciplina CI1001 - Programacao I
 * Autor: Marcos Castilho
 * Data: 02/09/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include "liblef.h"

#define COORD 1
#define VETOR 2
#define TAM_MAX_V 8

typedef struct coordenada {
    int x;
    int y;
} coordenada_t;

typedef struct vetor {
    int* vetor;
    int tam;
} vetor_t;

void destroi_coordenada_t (void *coord)
{}

void destroi_vetor_t (void *v)
{}

void imprime_lef (lef_t *l)
{
    evento_t* e;
    int i;

    printf ("LEF:\n");
    while ((e= obtem_primeiro_lef(l)))
    {
        printf ("tempo: %d ", e->tempo);
        printf ("tipo: %d ", e->tipo);
        printf ("tamanho: %d ", e->tamanho);
        int aux = ((vetor_t*)e->dados)->tam;
        printf("e->dados->tam: %d\n", aux);
        if (e->tipo == COORD)
            printf ("COORD: (%d,%d)\n", ((coordenada_t*)e->dados)->x, ((coordenada_t*)e->dados)->y);
        else if (e->tipo == VETOR)
        {
            printf ("VETOR: [");
            for (i=0; i<((vetor_t*)e->dados)->tam-1; i++){
                printf ("%d, ", ((vetor_t*)e->dados)->vetor[i]);
            }
            printf ("%d]\n", ((vetor_t*)e->dados)->vetor[((vetor_t*)e->dados)->tam-1]);
        }
    }
    printf ("\n");

    return;
}

/* retorna 1 se criou coordenada, zero em caso de falha */
int insere_dados_evento_coordenada (evento_t* e, int tempo, int tipo, int x, int y)
{
    e->tempo= tempo;
    e->tipo= tipo;
    e->tamanho= sizeof (coordenada_t);
    ((coordenada_t*)e->dados)->x= x;
    ((coordenada_t*)e->dados)->y= y;
    e->destroidados = destroi_coordenada_t;

    return 1;
}

/* retorna 1 se criou coordenada, zero em caso de falha */
int insere_dados_evento_vetor (evento_t* e, int tempo, int tipo, int tam)
{
    int i;

    e->tempo= tempo;
    e->tipo= tipo;
    e->tamanho= sizeof (vetor_t);
    e->destroidados = destroi_vetor_t;

    ((vetor_t*) e->dados)->tam= tam;
    for (i= 0; i < tam; i++){
        ((vetor_t*) e->dados)->vetor[i]= i;
    }

    return 1;
}

int aloca_dados_coordenada (evento_t* e)
{
    if (! (e->dados= (coordenada_t*) malloc (sizeof(coordenada_t))))
        return 0;

    return 1;
}

int aloca_dados_vetor (evento_t* e, int tam)
{
    if (! (e->dados= (vetor_t*) malloc (sizeof(vetor_t))))
        return 0;

    if (! (((vetor_t*) e->dados)->vetor = (int*) malloc (sizeof (int)*tam)))
    {
        return 0;
    }
    ((vetor_t*) e->dados)->tam = tam;

    return 1;
}

int main (void) {
    lef_t *l;
    evento_t ev_coord;
    evento_t ev_vetor;

    aloca_dados_coordenada (&ev_coord);
    aloca_dados_vetor (&ev_vetor, TAM_MAX_V);

    printf ("---> Teste criar LEF\n");
    if (! (l= cria_lef ())) {
        printf ("ERRO: falha na criacao da LEF\n");
        return 1;
    }
    printf("Lef criada com sucesso.\n");
    printf("\n");

    /* ------------------ teste inserir no inicio LEF ---------------- */
    printf ("---> Teste insere no inicio\n");
    insere_dados_evento_coordenada (&ev_coord, 0, COORD, 0, 0);
    if (! (adiciona_inicio_lef (l, &ev_coord)))
        printf ("ERRO: falha na insercao do evento na LEF\n");

    insere_dados_evento_vetor (&ev_vetor, 1, VETOR, TAM_MAX_V);
    if (! (adiciona_inicio_lef (l, &ev_vetor)))
        printf ("ERRO: falha na insercao do evento na LEF\n");

    insere_dados_evento_coordenada (&ev_coord, 0, COORD, 1, 1);
    if (! (adiciona_inicio_lef (l, &ev_coord)))
        printf ("ERRO: falha na insercao do evento na LEF\n");

    insere_dados_evento_vetor (&ev_vetor, 1, VETOR, TAM_MAX_V-3);
    if (! (adiciona_inicio_lef (l, &ev_vetor)))
        printf ("ERRO: falha na insercao do evento na LEF\n");

    imprime_lef (l);

    /* ------------------ teste inserir ordenado na LEF -------------- */
    printf ("---> Teste insere ordenado\n");
    /* insercao de coordenadas */

    insere_dados_evento_coordenada (&ev_coord, 5, COORD, 0, 0);
    if (! (adiciona_ordem_lef (l, &ev_coord)))
        printf ("ERRO: falha na insercao do evento na LEF\n");

    insere_dados_evento_coordenada (&ev_coord, 6, COORD, 1, 1);
    if (! (adiciona_ordem_lef (l, &ev_coord)))
        printf ("ERRO: falha na insercao do evento na LEF\n");

    insere_dados_evento_vetor (&ev_vetor, 1, VETOR, TAM_MAX_V);
    if (! (adiciona_ordem_lef (l, &ev_vetor)))
        printf ("ERRO: falha na insercao do evento na LEF\n");

    insere_dados_evento_vetor (&ev_vetor, 3, VETOR, TAM_MAX_V-3);
    if (! (adiciona_ordem_lef (l, &ev_vetor)))
        printf ("ERRO: falha na insercao do evento na LEF\n");

    imprime_lef (l);

    /* ------------------ teste destruir LEF --------------------- */
    /* printf ("---> Teste destruir lef, testar leaks com valgrind\n");
    l= destroi_lef (l); */

    return 0;
}
