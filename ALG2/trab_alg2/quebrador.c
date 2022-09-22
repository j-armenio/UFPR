#include <stdio.h>
#include "biblioteca.h"
#include "quebrador-senha.h"
#define MAX 6

/* retorna 1 caso seja igual e 0 caso contrario */
int compara_vetor(char vetor[], char senha[]){
    int aux = 0;
    for (int j = 0; j < 6; j++){
        if(vetor[j] == senha[j]){
            aux++;
            if (aux == 6)
                return 1;
        } 
        else    
            return 0;
    }
}  

int backtracking_exaustivo(char *vetor, int tam){
    if (tam != MAX){
        /* varia as letras */
        for (char i = 97; i <= 122; i++){
            vetor[tam] = i;
            login(vetor);
            printf("%s\n", vetor);
            /* porque ele troca com o indice inverso?? */
            backtracking_exaustivo(vetor, tam + 1);
            /* como caralhos ele passa dessa recursao?? ele n devia chegar nesse if nunca */
            if (compara_vetor(vetor, senha_atual()))
        	    return 1;
        }
        /* varia os numeros */
        for (char i = 48; i <= 57; i++){
            vetor[tam] = i;
            login(vetor);
            printf("%s\n", vetor);
            backtracking_exaustivo(vetor, tam + 1);
            if (compara_vetor(vetor, senha_atual()))
          	    return 1;
        }
    }
}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebrador_senha_exaustivo(){
    int tam = 0;
    char *vetor = malloc(sizeof(char) * MAX);
    vetor[0] = "a";
    if(backtracking_exaustivo(vetor, tam))
        return 1;
    return 0;
}

/* 
tem que verificar 3 coisas: 
    repeticao de caracteres -- 
    a senha deve conter pelo menos 2 letras -- guardar a quantidade de numeros atuais
    a senha deve conter pelo menos 2 numeros -- guardar a quantidade de letras atuais
*/
int verifica_poda(char vetor[], char ult_add, int rep, int n_letra, int n_numero){
    /* rep eh 1 se ha uma repeticao e 0 caso contrario */
    for (int i = 0; i < MAX; i++){
        for (int j = i + 1; j < MAX; j++){
            if (vetor[j] == vetor[i])
                rep = 1;
        }
    }

    /* verifica se eh uma letra */
    if (ult_add >= 97 && ult_add <= 122){ /* eh uma letra */
        n_letra++;
    } 
    else{ /* eh um numero */
        n_numero++;
    }
}

int backtracking_poda(char *vetor, int tam, int rep, int n_letra, int n_numero){
    if (compara_vetor(vetor, senha_atual())) /* achou a senha */
        return 1;
    else{
        for (char i = 97; i <= 122; i++){
            vetor[tam] = i;
            login(vetor);
            // printf("%s\n", vetor);
            verifica_poda(vetor, vetor[tam], rep, n_letra, n_numero);
            /* if (tam > 4 AND n_letra < 2 OR n_numero < 2)
                    backtracking e poda
             OR rep != 0
                    backtracking e poda
            */
            backtracking_poda(vetor, tam + 1, rep, n_letra, n_numero);
            if (compara_vetor(vetor, senha_atual()))
        	    return 1;
        }
        for (char i = 48; i <= 57; i++){
            vetor[tam] = i;
            login(vetor);
            // printf("%s\n", vetor);
            verifica_poda(vetor, vetor[tam], rep, n_letra, n_numero);
            backtracking_poda(vetor, tam + 1, rep, n_letra, n_numero);
            if (compara_vetor(vetor, senha_atual()))
          	    return 1;
        }
    }
}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int quebrador_senha_poda(){
    char *vetor = malloc(sizeof(char) * MAX);
    vetor[0] = 97;
    int tam = 0;
    int n_letra = 0;
    int n_numero = 0;
    int rep = 0; /* flag para repeticao de termos */
    if(backtracking_poda(vetor, tam, rep, n_letra, n_numero))
        return 1;
    return 0;
}