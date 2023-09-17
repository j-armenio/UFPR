#include <stdio.h>
#include <stdlib.h>

#include "libaux.h"

int main()
{
    double numbers[NUMBERS_AMOUNT];   // 5
    char operators[OPERATORS_AMOUNT]; // 4

    readInput(numbers, operators);

    // Cria intervalos
    doubleRange *ranges = (doubleRange *)malloc(sizeof(doubleRange) * NUMBERS_AMOUNT);
    createRanges(numbers, ranges);



}