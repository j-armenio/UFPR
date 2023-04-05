#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 1024

/* Ex 1 */
void inverter_string(){
    int i, j;
    char str[TAM];

    scanf("%s", str);

    j = strlen(str);

    for (i = 0; i <= strlen(str); i++){
        printf("%c", str[j]);
        j--;
    }

    printf("\n");
}

/* Ex 2 */
void tamanho_string(){
    char str[TAM];
    char c;
    int i;

    scanf("%s", str);

    i = 0;
    c = str[0];

    while (c != '\0'){
        i++;
        c = str[i];
    }

    printf("%d\n", i);
}

/* Ex 3 */
void converter_minusculas(){
    char str[TAM];
    int i = 0;

    scanf("%s", str);

    while (str[i] != '\0'){
        if (str[i] < 97)
            str[i] += 32;
        i++;
    }

    printf("%s\n", str);
}

int main(){
    converter_minusculas();
}