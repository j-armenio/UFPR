#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "quebrador-senha.h"
#define MAX 6

int quebrador_senha_exaustivo(){
    if (1 == 1)
        return 1;
    return 0;
}

/* retorna 1 caso seja igual e 0 caso contrario */
int compara_com_senha(char *vetor){
    if (strcmp(vetor, senha_atual()) == 0)
        return 1;
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

/* verifia o ultimo char adicionado e atualizado os contadores de letra e numero */
void atualiza_conts(char *vetor, int n_letras, int n_numeros){
    for (int i = 0; i < 6; i++)
        if (vetor[i] >= 97) // eh uma letra
            n_letras++;
        else if (vetor[i] <= 57) // eh um numero
            n_numeros++;
}

/* retorna 0 em caso de restricao e 1 caso contrario */
int verifica_restricoes(char *vetor, int tam, int n_letras, int n_numeros){
    /* retorna 0 caso haja repeticao */
    if (repeticao(vetor))
        return 0;

    atualiza_conts(vetor, n_letras, n_numeros);

    if (tam < 5)
        return 1;
    
    if (n_letras >= 2 && n_numeros >= 2)
        return 1;
    return 0;
}

int backtracking_poda(char *vetor, int tam, int n_letras, int n_numeros){
    if (tam == MAX){
        if (compara_com_senha(vetor))
            return 1;
        }else{
        /* varia as letras */
        for (char i = 97; i <= 122; i++){
            vetor[tam] = i;
            login(vetor);
            // printf("%s\n", vetor);
            if (verifica_restricoes(vetor, tam, n_letras, n_numeros))
                if (backtracking_poda(vetor, tam, n_letras, n_numeros))
                    return 1;
        }
        /* varia os numeros */
        for (char i = 48; i <= 57; i++){
            vetor[tam] = i;
            login(vetor);
            // printf("%s\n", vetor);
            if (verifica_restricoes(vetor, tam, n_letras, n_numeros))
                if (backtracking_poda(vetor, tam, n_letras, n_numeros))
                    return 1;            
        }
    }
    return 0;
}

int quebrador_senha_poda(){
    char *vetor = malloc(sizeof(char) * MAX);
    int tam = 0;         /* indice de onde esta variando */
    int n_letras = 0;    /* numero de letras no vetor */
    int n_numeros = 0;   /* numero de numeros no vetor */

    backtracking_poda(vetor, tam, n_letras, n_numeros);

    free(vetor);
}