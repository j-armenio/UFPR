#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

void escreve_ao_contrario(char str[], char str2[]){
    int i;
    int j = 0;

    scanf("%s", str);

    for (i=strlen(str)-1; i >= 0; i--){
        str2[j] = str[i];
        j++;
    }
    printf("%s\n", str2);
}

void calcula_tamanho_str(char str[]){
    char c = '0';
    int i = -1;

    scanf("%s", str);

    while (c != '\0'){
        i++;
        c = str[i];
    }

    printf("%d\n", i);
}

void converte_em_maiusculo(char str[]){
    int i;
    char c = '0';

    scanf("%s", str);

    // Percorre a str
    for (i = 0; i < strlen(str); i++){
        c = str[i];
        /* Verifica se eh letra minusculo */
        if ((c > 96) && (c < 123)){
            str[i] = c - 32;
        }
    }
    printf("%s\n", str);
}

void sort(){ /* ??? */

}

void remove_certosCaracteres(char str[]){
    int i, temp;
    char c;

    scanf("%s", str);

    for (i = 0; i < strlen(str); i++){
        // printf("%d\n", i);
        c = str[i];
        // printf("%d\n", c);

        if (((c > 32) && (c < 47)) || ((c > 57) && (c < 65)) || ((c > 90) && (c < 97)) || (c > 122)){
            // printf("É ESSE NOIA AQUI: %c\n", c);
            for (temp = i; temp < strlen(str); temp++)
                str[temp] = str[temp + 1];
            i--;
        }

    }
    printf("%s\n", str);
}

void remove_repetidos_em_sequencia(char str[]){
    int i, temp;

    scanf("%s", str);

    for (i = 0; i < strlen(str); i++){
        if (str[i] == str[i + 1]){
            for (temp = i; temp < strlen(str); temp++){
                str[temp] = str[temp + 1];
            }
            i--;
        }
    }
    printf("%s\n", str);
}

void colocar_colchetes_certosCaracteres(char str[]){ /* dando segfault */
    int i, temp, aux2;
    char c, aux;

    scanf("%s", str);

    for (i = 0; i < strlen(str); i++){
        c = str[i];
        if (((c > 32) && (c < 47)) || ((c > 57) && (c < 65)) || ((c > 90) && (c < 97)) || (c > 122)){
            for (temp = strlen(str); temp > i; temp--){
                str[temp + 2] = str[temp];
            }
            /* dando segfault nisso aq embaixo sla pq
            str[i + 1] = str[i];
            str[i] = 32;
            str[i + 2] = 32;
            */
            // i--; 
        }
    }
    printf("%s\n", str);

}

int busca(char agulha[], char palheiro[]){ /* mal comecei, implementar strstr */
    scanf("%s", agulha);
    scanf("%s", palheiro);

    
}

int main(){
    char str[SIZE+1];
    char str2[SIZE+1];

    busca(str, str2);
}