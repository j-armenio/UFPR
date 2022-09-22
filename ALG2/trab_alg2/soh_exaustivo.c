#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "quebrador-senha.h"
#define MAX 6

/* retorna 1 caso seja igual e 0 caso contrario */
int compara_com_senha(char *vetor){
    if (strcmp(vetor, senha_atual()) == 0)
        return 1;
    return 0;
}  

int backtracking_exaustivo(char *vetor, int tam){
    if (tam == MAX){
        if (compara_com_senha(vetor))
        	    return 1;
    }else{
        /* varia as letras */
        for (char i = 97; i <= 122; i++){
            vetor[tam] = i;
            login(vetor);
            // printf("%s\n", vetor);
            /* porque ele troca com o indice inverso?? */
            backtracking_exaustivo(vetor, tam + 1);
        }
        /* varia os numeros */
        for (char i = 48; i <= 57; i++){
            vetor[tam] = i;
            login(vetor);
            // printf("%s\n", vetor);
            backtracking_exaustivo(vetor, tam + 1);
        }
    }
}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebrador_senha_exaustivo(){
    int tam = 0;
    char *vetor = malloc(sizeof(char) * MAX);
    // char vetor[6] = "";
    if(backtracking_exaustivo(vetor, tam))
        return 1;
    return 0;
}

int backtracking_poda(char vetor[], int tam){

}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebrador_senha_poda(){
    if (1 == 1)
        return 1;
    return 0;
}