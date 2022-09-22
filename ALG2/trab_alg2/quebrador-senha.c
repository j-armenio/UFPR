#include <string.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "quebrador-senha.h"
#define MAX 6

int compara_com_senha(char *vetor){
    if (strcmp(vetor, senha_atual()) == 0)
        return 1;
    return 0;
}   

int backtracking_exaustivo(char vetor[], int tam){
    if (tam != MAX){ 
        for (char i = 97; i <= 122; i++){
            vetor[tam] = i;
            login(vetor);                          
            backtracking_exaustivo(vetor, tam + 1);
            if (compara_com_senha(vetor))
        	    return 1;
        }
        for (char i = 48; i <= 57; i++){
            vetor[tam] = i;
            login(vetor); 
            backtracking_exaustivo(vetor, tam + 1);
            if (compara_com_senha(vetor))
          	    return 1;
        }
    }
    return 0;
}

/* retorna 1 caso tenha repeticao e 0 caso contrario */
int repeticao(char *vetor){
    for (int i = 0; i < MAX; i++){
        for (int j = i + 1; j < MAX; j++){
            if (vetor[j] == vetor[i])
                return 1;
        }
    }
    return 0;
}

int verifica_restricoes(char *vetor, int tam, int n_letras, int n_numeros, int ult_add){
    if (tam < 5)
        return 1;
        
    if (repeticao(vetor))
        return 0;

    if (n_letras >= 2 && n_numeros >= 2)
        return 1;
    return 0;
}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebrador_senha_exaustivo(){
    int tam = 0;
    char vetor[6];
    if(backtracking_exaustivo(vetor, tam))
        return 1;
    return 0;
}

int backtracking_poda(char *vetor, int tam, int n_letras, int n_numeros){
    if (tam != MAX){
        /* varia as letras */
        n_letras++;
        for (char i = 97; i <= 122; i++){
            vetor[tam] = i;
            if (verifica_restricoes(vetor, tam, n_letras, n_numeros, i)){
                login(vetor);
                backtracking_poda(vetor, tam + 1, n_letras, n_numeros);
                if (compara_com_senha(vetor))
                    return 1;
            }
        }
        n_letras--;
        n_numeros++;
        /* varia os numeros */
        for (char i = 48; i <= 57; i++){
            vetor[tam] = i;
            if (verifica_restricoes(vetor, tam, n_letras, n_numeros, i)){
                login(vetor);
                backtracking_poda(vetor, tam + 1, n_letras, n_numeros);
                if (compara_com_senha(vetor))
                    return 1;      
            }    
        }
    }
    return 0;
}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebrador_senha_poda(){
    char *vetor = malloc(sizeof(char) * MAX);
    int tam = 0;         /* indice de onde esta variando */
    int n_letras = 0;    /* numero de letras no vetor */
    int n_numeros = 0;   /* numero de numeros no vetor */

    if(backtracking_poda(vetor, tam, n_letras, n_numeros))
        return 1;
    return 0;
    
    free(vetor);
}