#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myht.h"

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

    /* DAR MALLOCA NAS TABLE */

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

    /* printf("pos1:%d pos2:%d\n", pos1, pos2); */

    /* Verifica se a pos na T1 esta ocupada */
    if ((T1[pos1].data == 0) || (T1[pos1].data == -1)){
        T1[pos1].data = key;
        T1[pos1].table = "T1";
    }
    else{ /* se já estiver ocupada, colocar na T2 */
        posaux = floor(TABLE_SIZE * (T1[pos1].data * 0.9 - floor(T1[pos1].data * 0.9)));
        T2[posaux].data = T1[pos1].data;
        T1[pos1].data = key;
        T1[pos1].table = "T2";
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


void printTables(HtItem *T1, HtItem *T2)
{
    int i, j;
    int count = 0;
    char *curTb = malloc(sizeof(char) * 3);

    /* conta a quantidade de impressões que devem ser feitas*/
    for (i = 0; i < TABLE_SIZE; i++){
        if ((T1[i].data != -1) && (T1[i].data != 0)){
            count++;
        }
        if ((T2[i].data != -1) && (T2[i].data != 0)){
            count++;
        }
    }

    /* Pega a menor chave da tabela */
    int menor = T1[0].data;
    for (i = 1; i < TABLE_SIZE; i++){
        if (T1[i].data < menor){
            menor = T1[i].data;
            curTb = "T1";
        }
    }

    for (i = 0; i < count; i++){
        for (j = 0; j < TABLE_SIZE; j++){
            if (T1[j].data < menor){
                menor = T1[j].data;
                curTb = "T1";
            }
            if (T2[j].data < menor){
                menor = T2[j].data;
                curTb = "T2";
            }
        }
        printf("%d,%s,%d\n", menor, curTb, j);
    }
}