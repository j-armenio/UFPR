/*
 * Testes para a libfila.c
 * Disciplina CI1001 - Programacao I
 * Autor: Marcos Castilho
 * Data: 03/08/2022
*/

#include <stdio.h>
#include "libfila.h"

/* nao precisa mais do que MAX_TAM=5 para testar esta biblioteca */
#define MAX_TAM 5

fila_t* teste_criar_fila ()
{
    fila_t* f;

    if (! (f = cria_fila ()))
        fprintf (stderr, "Falha na alocacao da fila\n");

    printf ("Fila criada com sucesso\n");
    printf ("tamanho da fila eh: %d\n", tamanho_fila (f));
    return f;
}

void teste_fila_vazia (fila_t* f)
{
    if (fila_vazia (f))
        printf ("fila vazia\n");
    else
        printf ("fila nao esta vazia\n");
    printf ("tamanho da fila eh: %d\n", tamanho_fila (f));
}

/* insere nelem elementos na lista, esperando que acuse pilha cheia */
void teste_queue (fila_t* f, int nelem)
{
    int i;

    for (i=1; i <= nelem; i++)
    {
        if (! insere_fila (f, i))
            printf ("Fila cheia, nao inseriu %d\n", i);
        else
            printf ("Enfileirou %d\n", i);
        if (fila_vazia (f))
            printf ("ooops: fila nao deveria estar vazia aqui\n");
    }
    printf ("tamanho da fila eh: %d\n", tamanho_fila (f));
}

void teste_dequeue (fila_t* f)
{
    int i;

    /* remove todo mundo */
    printf ("Desenfileirando todos os elementos\n");
    while (! fila_vazia (f))
    {
        if (retira_fila (f, &i))
            printf ("removeu %d\n", i);
    }
    printf ("tamanho da fila eh: %d\n", tamanho_fila (f));
}

/* Este teste eh mais complicado. Pensei assim:
 * insere max_elementos na lista, depois remove e insere mais alguns 
 * alternadamente, o tamanho tem que ficar estavel, nao eh para dar 
 * pilha cheia, nem pilha vazia */
void teste_fila_circular (fila_t* f, int nelem)
{
    int i, ini;

    for (i=1; i <= nelem; i++)
    {
        if (! insere_fila (f, i))
            printf ("Fila cheia, nao inseriu %d\n", i);
        else
            printf ("Enfileirou %d\n", i);
        if (fila_vazia (f))
            printf ("ooops: fila nao deveria estar vazia aqui\n");
    }
    for (i=6; i <= 10; i++)
    {
        if (! retira_fila (f, &ini))
            printf ("ERRO: deveria ter removido %d", ini);
        else
        {
            printf ("removeu %d ; ", ini);
            insere_fila (f, i);
            printf ("inseriu %d ; ", i);
        }
        printf ("tamanho da fila: %d\n", tamanho_fila (f));
    }
}

void imprime_fila (fila_t* f)
{
    int i, n, ini;

    n= tamanho_fila (f);
    printf ("tam %d: fila:  ",n);
    for (i=0; i < n; i++)
    {
        if (! retira_fila (f, &ini))
            printf ("ERRO: deveria ter removido %d", ini);
        else
        {
            printf ("%d ", ini);
            insere_fila (f, ini);
        }
    }
    printf ("\n");
}

int main (void)
{
    fila_t *f;
    int i;

    printf ("------------> Teste 1: cria fila e testa se esta vazia\n");
    f= teste_criar_fila ();
    teste_fila_vazia (f);
    printf ("--> Esperado: tem que dar fila vazia com tamanho zero\n");
    printf ("\n");

    printf ("------------> Teste 2: enfileira alguns elementos\n");
    teste_queue (f, MAX_TAM); 
    printf ("--> Esperado: mostrar que enfileirou de 1 a 5.\n");
    teste_fila_vazia (f);
    printf ("--> Esperado: fila nao vazia\n");
    printf ("--> Esperado: tamanho tem que ser 5.\n");
    printf ("\n");

    printf ("------------> Teste 3: desenfileira ate fila ficar vazia\n");
    teste_dequeue (f); 
    printf ("--> Esperado: mostrar remocao de 1 ate 5\n");
    printf ("--> Esperado: tamanho tem que ser 0.\n");
    printf ("\n");

    printf ("------------> Teste 4: testa se fila ficou de fato vazia\n");
    teste_fila_vazia (f);
    printf ("--> Esperado: fila tem que estar vazia\n");
    printf ("--> Esperado: tamanho tem que ser 0.\n");
    printf ("\n");

    printf ("------------> Teste 5: tenta dequeue de fila vazia\n");
    if (! retira_fila (f, &i))
        fprintf (stderr, "--> Esperado: nao ter dado segmentation fault\n");
    printf ("\n");

    printf ("------------> Teste 6: destruir fila vazia\n");
    f= destroi_fila (f);
    printf ("--> Esperado: fila vazia destruida, verificar no valgrind se deu leak\n");
    printf ("\n");

    printf ("------------> Teste 7: destruir fila com elementos presentes\n");
    f= teste_criar_fila ();
    teste_queue (f, MAX_TAM); 
    f= destroi_fila (f);
    printf ("--> Esperado: fila com elementos destruida, verificar no valgrind se deu leaks\n");
    printf ("\n");

    printf ("------------> Teste 8: enfileira alguns elementos de novo para mais testes\n");
    f= teste_criar_fila (MAX_TAM);
    teste_queue (f, MAX_TAM); 
    printf ("--> Esperado: mostrar que enfileirou de 1 a 5.\n");
    printf ("--> Esperado: tamanho tem que ser 5.\n");
    printf ("\n");

    printf ("------------> Teste 9: imprime a fila\n");
    imprime_fila (f);
    printf ("--> Esperado: imprime tam: 5 fila: 1 2 3 4 5\n");
    printf ("\n");

    printf ("------------> Teste 10: remove tudo de novo\n");
    teste_dequeue (f); 
    printf ("--> Esperado: removeu 1 2 3 4 5 e o tamanho ficou zero\n");
    printf ("\n");

    printf ("------------> Teste 11: imprime uma fila vazia\n");
    imprime_fila (f);
    printf ("--> Esperado: imprime tam: 0 fila:\n");
    printf ("\n");

    printf ("------------> Teste 12: Final: libera a fila sem erro no valgrind\n");
    f= destroi_fila (f);

    return 0;
}
