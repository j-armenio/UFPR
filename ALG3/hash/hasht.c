#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hasht.h"

#define TABLE_SIZE 11

/* 
    Estado da tabela:
    0   NÃO INICIADA 
    -1  EXCLUIDA
*/

/* ----------------- Funções Locais ----------------- */

void mustInit(void *test, char *description)
{
    if (test != NULL)
        return;

    printf("Couldn't initializa %s.\n", description);
    exit(1);
}

/* ----------------- Funções Globais ----------------- */

HtItem *createTable()
{
    HtItem *HsT;
    int i;

    HsT = malloc(sizeof(HtItem) * TABLE_SIZE);
    mustInit(HsT, "Tabela");

    for (i = 0; i < TABLE_SIZE; i++){
        HsT[i].data = 0;
        HsT[i].table = 0;
    }

    return HsT;
}

void freeTables(HtItem *T1, HtItem *T2)
{
    free(T1);
    free(T2);
}

int insertHt(HtItem *T1, HtItem *T2, int key)
{
    int pos1 = key % TABLE_SIZE;
    int posaux;

    /* Verifica se a pos na T1 esta ocupada */
    if ((T1[pos1].data == 0) || (T1[pos1].data == -1)){
        T1[pos1].data = key;
        T1[pos1].table = 1;
        T1[pos1].index = pos1;
    }
    else{ /* se já estiver ocupada, colocar na T2 */
        posaux = floor(TABLE_SIZE * (T1[pos1].data * 0.9 - floor(T1[pos1].data * 0.9)));
        T2[posaux].data = T1[pos1].data;
        T2[posaux].table = 2;
        T2[posaux].index = posaux;
        T1[pos1].data = key;
        T1[pos1].table = 1;
        T1[pos1].index = pos1;
    }
    return 1;
}

int removeHt(HtItem *T1, HtItem *T2, int key)
{
    int pos1 = key % TABLE_SIZE;
    int pos2 = floor(TABLE_SIZE * (key * 0.9 - floor(key * 0.9)));

    if ((T2[pos2].data != 0) && (T2[pos2].data) != -1) /* Verifica se a chave esta em T2 */
        T2[pos2].data = -1;
    else if ((T1[pos1].data != 0) && T1[pos1].data != -1) /* Verifica se a chave esta em T1 */
        T1[pos1].data = -1;

    return 1;
}

int compareItens(const void *a, const void *b)
{
    HtItem *item1 = (HtItem *) a;
    HtItem *item2 = (HtItem *) b;
    
    return (item1->data - item2->data);
}

void printTables(HtItem *T1, HtItem *T2)
{
    int i, j;
    j = 0;

    HtItem *resultTable = malloc(sizeof(HtItem) * TABLE_SIZE * 2);

    /* Insere todas chaves de ambas tabelas em um array */
    for (i = 0; i < TABLE_SIZE; i++){
        if ((T1[i].data != 0) && (T1[i].data != -1)){
            resultTable[j] = T1[i];
            j++;
        }
    }
    for (i = 0; i < TABLE_SIZE; i++){
        if ((T2[i].data != 0) && (T2[i].data != -1)){
            resultTable[j] = T2[i];
            j++;
        }
    }

    qsort(resultTable, TABLE_SIZE, sizeof(HtItem), compareItens);

    /* Imprime os valores do array */
    for (i = 0; i < TABLE_SIZE; i++){
        if ((resultTable[i].data != 0) && (resultTable[i].data != -1)){
            printf("%d,T%d,%d\n", resultTable[i].data, resultTable[i].table, resultTable[i].index);
        }
    }
}