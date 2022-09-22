#ifndef _LIBPILHA_H
#define _LIBPILHA_H

typedef struct {
   char *p_elems;   /* espaco armazenamento dos elementos */
   int p_nelem;     /* qtde máxima de elementos */
   int p_topo;      /* elelemento do topo da pilha */
} PILHA;

/* Cria e retorna uma nova pilha com capacidade de nelem. Retornal NULL(0) em caso de 
erro de alocação */
PILHA *piCria(int nelem);

/* Insere o char elem na pilha. Retorna o número de elementos na pilha em caso de sucesso
e -1 em caso a pilha esteja cheia */
int piPush(PILHA* pilha, char elem);

/* Remove e retorna um elemento da pilha. Em caso de pilha vazia retorna 0 */
char piPop(PILHA* pilha);
 
/* Similar ao Pop, mas retorna sem remover */
char piTop(PILHA* pilha);

/* return 1 se pilha vazia, 0 em outros casos */ 
int piVazia(PILHA* pilha);

/* desaloca memoria de PILHA */
void piDestroi(PILHA* pilha);

#endif