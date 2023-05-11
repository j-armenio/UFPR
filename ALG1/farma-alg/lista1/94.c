#include <stdio.h>

void main()
{
    int preco, desconto;

    scanf("%i %i", &preco, &desconto);

    printf("%d\n", preco - desconto);
}