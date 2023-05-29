#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *input;
    FILE *output;
    char *buffer;

    buffer = malloc(sizeof(char) * 4);
    if (! buffer) {
        printf("Erro ao alocar buffer\n");
        return 1;
    }

    input = fopen("input.txt", "rb");
    if (! input) {
        printf("Erro ao abrir arquivo de entrada\n");
        return 1;
    }

    output = fopen("output.txt", "wb");
    if (! output) {
        printf("Erro ao abrir arquivo de saída\n");
        return 1;
    }

    while (! feof(input)) {
        fread(buffer, sizeof(char), 4, input);
        printf("%s", buffer);
        fwrite(buffer, sizeof(char), 4, output);
    }
    printf("\n");

}