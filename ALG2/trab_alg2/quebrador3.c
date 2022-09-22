#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "quebrador-senha.h"
#define MAX 6

/* retorna 1 se o vetor eh igual a senha e 0 caso contrario */
int compara_com_senha(char *vetor){
    if (strcmp(vetor, senha_atual()) == 0)
        return 1;
    return 0;
}

int backtracking_exaustivo(char *vetor, int tam){
}

int quebrador_senha_exaustivo(){
    if (1 == 1)
        return 1;
    return 0;
}

int verifica_poda(char *vetor, int rep){
    /* caso ache uma repeticao no vetor rep recebe 1 */
    for (int i = 0; i < MAX; i++){
        for (int j = i + 1; j < MAX; j++){
            if (vetor[j] == vetor[i])
                rep = 1;
        }
    }
    return rep;
}

int backtracking_poda(char *vetor, int tam, int rep){
    if (tam != MAX){
        /* varia as letras */
        for (char i = 97; i <= 122; i++){
            vetor[tam] = i;
            login(vetor);
            // printf("%s\n", vetor);
            
            backtracking_poda(vetor, tam + 1, rep);
            if (compara_com_senha(vetor))
        	    return 1;
        }
        /* varia os numeros */
        for (char i = 48; i <= 57; i++){
            vetor[tam] = i;
            login(vetor);
            // printf("%s\n", vetor);

            backtracking_poda(vetor, tam + 1, rep);
            if (compara_com_senha(vetor))
          	    return 1;
        }
    }
}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebrador_senha_poda(){
    char *vetor = malloc(sizeof(char) * MAX);
    int tam = 0;
    int rep = 0;
    free(vetor);
}