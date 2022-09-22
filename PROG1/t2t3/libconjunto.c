#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"
#include "libaleat.h"

/* Ordena o vetor do conjunto c. */
void insertion_sort(conjunto_t *c)
{
    int i, j, aux;

    for (i = 1; i < c->card; i++){
        aux = c->v[i];
        j = i - 1;
        while ((j >= 0) && (c->v[j] > aux)){
            c->v[j + 1] = c->v[j];
            j = j -1;
        }
        c->v[j + 1] = aux;
    }
}

/* Retorna o indice de x ou -1 caso ele nao exista. */
int busca_binaria(conjunto_t *c, int x)
{
    /* busca binaria so em conjuntos ordenados */
    int ini = 0;
    int fim = c->card-1;
    int meio = (ini + fim) / 2; /* apontador que procura o indice de x */

    while ((ini <= fim) && (x != c->v[meio])){
        if (x < c->v[meio]){
            fim = meio -1;
        }
        else{ /* x eh maior que v[meio] */
            ini = meio +1;
        }
        meio = (ini + fim) / 2;
    }
    if (fim < ini) /* nao achou */
        return -1;
    else
        return meio;
}

/* Cria um conjunto vazio com tamanho maximo max e o retorna, se falhar retorna NULL. */
conjunto_t *cria_conjunto (int max)
{
    conjunto_t *c;

    /* aloca espaco para struct conjunto_t */
    if (!(c = malloc(sizeof(conjunto_t))))
        return NULL;
    /* aloca espaco para vetor */
    if (!(c->v = malloc(sizeof(int) * max)))
        return NULL;
    
    c->card = 0;
    c->max = max;

    return c;
}

/* Remove todos os elementos do conjunto, libera espaco e devolve NULL. */
conjunto_t *destroi_conjunto (conjunto_t *c)
{
    /* NAO VAO COBRAR */
    return  NULL;
}

/* Retorna 1 se o conjunto esta vazio e 0 caso contrario. */
int conjunto_vazio (conjunto_t *c)
{
    if (c->card == 0)
        return 1;
    return 0;
}

/* Retorna numero de elementos no conjunto. */
int cardinalidade (conjunto_t *c)
{
    return c->card;
}

/* Insere elemento no conjunto c e retorna 1.
   Retorna 0 se elemento ja existir.
   Retorna -1 se vetor tiver cheio. */
int insere_conjunto (conjunto_t *c, int elemento)
{
    if (c->card + 1 > c->max)
        return -1;

    if (! (pertence(c, elemento))){
        c->v[c->card] = elemento;
        c->card++;
        insertion_sort(c);
        return 1;
    }
    return 0;
}

/* Remove elemento do conjunto e retorna 1. 
   Retorna 0 caso o elemento nao exista. */
int retira_conjunto (conjunto_t *c, int elemento)
{
    int i;
    int indice = busca_binaria(c, elemento);

    if (indice != -1){ /* achou o elemento em c */
        for (i = indice; i < c->card-1; i++)
            c->v[i] = c->v[i + 1];
        c->card--;
        return 1;
    }
    return 0;
}

/* Retorna 1 se elemento pertence a c e 0 caso contrario. */
int pertence (conjunto_t *c, int elemento)
{
    if (busca_binaria(c, elemento) == -1)
        return 0;
    return 1;
}

/* Retorna 1 se c1 esta contido em c2 e 0 caso contrario. */
int contido (conjunto_t *c1, conjunto_t *c2)
{
    if (c1->card > c2->card)
        return 0;
    else{
        int ok = 1;
        int i = 0;
        int j = 0;

        while ((i < c1->card) && (j < c2->card) && ok){
            if (c1->v[i] < c2->v[j])
                ok = 0;
            else if (c1->v[i] > c2->v[j])
                j++;
            else{
                i++; j++;
            }
        }

        if ((ok) && (i >= c1->card))
            return 1;
        else
            return 0;
    }
}

/* Retorna 1 se c1 eh igual a c2 e 0 caso contrario. */
int sao_iguais (conjunto_t *c1, conjunto_t *c2)
{
    if (c1->card != c2->card)
        return 0;
    else{
        int i = 0;
        while ((i <= c1->card) && (c1->v[i] == c2->v[i]))
            i++;

        if (i <= c1->card-1)
            return 0;
        else
            return 1;
    }
}

/* Elementos em c1 que nao existem em c2. */
conjunto_t *cria_diferenca (conjunto_t *c1, conjunto_t *c2)
{
    conjunto_t *dif = cria_conjunto(c1->card);
    int i;

    for (i = 0; i < c1->card; i++){
        if (!pertence(c2, c1->v[i]))
            insere_conjunto(dif, c1->v[i]);
    }
    return dif;
}

/* Elementos em comum entre c1 e c2. */
conjunto_t *cria_interseccao (conjunto_t *c1, conjunto_t *c2)
{
    conjunto_t *intersec = cria_conjunto(c1->card);
    int i = 0;

    for (i = 0; i < c1->card; i++){ /* percorre c1 */
        if (pertence(c2,c1->v[i]))
            insere_conjunto(intersec, c1->v[i]);
        }

    return intersec;
}

/* Junta todos elementos de c1 com c2. */
conjunto_t *cria_uniao (conjunto_t *c1, conjunto_t *c2)
{
    conjunto_t *uni = cria_conjunto(c1->card + c2->card);
    int i = 0;
    int j = 0;
    int l;

    while ((i < c1->card) && (j < c2->card)){
        if (c1->v[i] < c2->v[j]){
            insere_conjunto(uni, c1->v[i]);
            i++;
        }
        else if (c1->v[i] > c2->v[j]){
            insere_conjunto(uni, c2->v[j]);
            j++;
        }
        else{
            insere_conjunto(uni, c1->v[i]);
            i++; 
            j++;
        }
    }
    for (l = 1; l < c1->card; l++)
        insere_conjunto(uni, c1->v[l]);
    for (l = j; l < c2->card; l++)
        insere_conjunto(uni, c2->v[l]);

    return uni;
}

/* Retorna uma copia de c. */
conjunto_t *cria_copia (conjunto_t *c)
{
    int i;
    conjunto_t *cop = cria_conjunto(c->max);
    cop->card = c->card;

    for (i = 0; i < c->card; i++)
        cop->v[i] = c->v[i];
    
    return cop;
}

/* Cria subconjunto de tamanho n com elementos aleatorios de c.
   Se c for vazio retorna um subc vazio.
   Se n >= card de c retorna c. */
conjunto_t *cria_subconjunto (conjunto_t *c, int n)
{
    int i;
    int aux;
    conjunto_t *subc = cria_conjunto(c->card);

    if (n >= c->card)
        return c;
    if (conjunto_vazio(c))
        return subc;
    for (i = 0; i < n; i++){
        aux = ALEAT(0, c->card-1);
        if (insere_conjunto(subc, c->v[aux]) == 0)
            i--;
    }
    return subc;
}

/* Imprime vetor. */
void imprime (conjunto_t *c)
{
    int i;

    if (conjunto_vazio(c)){
        printf("Conjunto vazio.\n");
        return;
    }

    for (i = 0; i < c->card; i++)
        printf("%d ", c->v[i]);
    printf("\n");
}

/* Dobra o tamanho do vetor do conjunto, 1 caso sucesso e 0 caso contrario. */
int redimensiona (conjunto_t *c)
{
    if (!(c->v = realloc(c->v, sizeof(int) * c->max * 2))){
        printf("Erro ao realocar.\n");
        return 0;
    }
    c->max = c->max * 2;

    return 1;
}

/* ######################################################### */

/* Inicializa contador. */
void iniciar_iterador (conjunto_t *c)
{
    c->ptr = 0;
}

/* Aumenta ptr e retorna o apontado em *elemento. Retorna 0 se acabou conjunto. */
int incrementar_iterador (conjunto_t *c, int *elemento)
{
    if (c->ptr < c->card){
        *elemento = c->v[c->ptr];
        c->ptr++;
        return 1;
    }
    return 0;
}

/* Retorna elemento em ptr. */
int retirar_um_elemento (conjunto_t *c)
{
    c->card--;
    return c->v[c->card];
}