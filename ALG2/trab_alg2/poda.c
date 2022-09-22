#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 6

static char senha[6] = "afabio";
static unsigned int n_login = 0;

char *senha_atual(void){
    return senha;
}

unsigned int login(char *vetor){
    ++n_login;
	if (strcmp(vetor, senha) == 0)
        return 1;
    return 0;
}

int compara_com_senha(char *vetor){
    if (strcmp(vetor, senha_atual()) == 0)
        return 1;
    return 0;
}

int verifica_poda(char *vetor, int tam){ 
    for (int i = 0; i < tam; i++){
        for (int j = i + 1; j < tam; j++){
            if (vetor[j] == vetor[i])
                return 0;
        }
	}
    return 1;
}

int backtracking_poda(char *vetor, int tam, int n_letra, int n_num){
    if (tam == MAX){
    	printf("%s %d %d\n", vetor, n_letra, n_num);
       	n_letra = 0;
    	n_num = 0;
    }
    else{
		n_letra++;
        for (char i = 97; i <= 122; i++){
            vetor[tam] = i;
           // login(vetor);
            /* verifica as restricoes */
                /* verifica se eh uma letra ou numero */    
          //  if (verifica_poda(vetor, tam)){
           		backtracking_poda(vetor, tam + 1, n_letra, n_num);
           		if (compara_com_senha(vetor)) /* verifica se achou a senha */
           			return 1;
			//}

           	
                
        }

        /* percorre os numeros */
        n_letra--;
        n_num++;
        for (char i = 48; i <= 57; i++){
            vetor[tam] = i;
           // login(vetor);
            /* verifica as restricoes */
                /* verifica se eh uma letra ou numero */
          //  if (verifica_poda(vetor, tam)){
           		backtracking_poda(vetor, tam + 1, n_letra, n_num);
                if (compara_com_senha(vetor)) /* verifica se achou a senha */
            		return 1;
          //  }
        }

    }
}

/* Retorna 1 caso tenha encontrado a senha e 0 caso contrario */
int main(){
    char *vetor = malloc(sizeof(char) * MAX);
    int tam = 0;        /* tamanho do vetor */
    int rep = 0;        /* flag para repeticao */
    int n_letra = 0;    /* numero de letras */
    int n_num = 0;      /* numero de numeros */

    if (backtracking_poda(vetor, tam, n_letra, n_num))
        return 1;
    return 0;
    free(vetor);
}
