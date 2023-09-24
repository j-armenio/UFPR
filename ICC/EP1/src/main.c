#include <stdio.h>
#include <stdlib.h>

#include "libaux.h"

int main()
{
    float numbers[NUMBERS_AMOUNT];   // 5
    char operators[OPERATORS_AMOUNT]; // 4

    readInput(numbers, operators);

    floatRange *ranges = (floatRange *)malloc(sizeof(floatRange) * NUMBERS_AMOUNT);
    createRanges(numbers, ranges);
    printRanges(ranges);
    printf("\n");

    solveOperations(ranges, operators);

    return 0;
}