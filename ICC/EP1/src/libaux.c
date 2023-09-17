#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "libaux.h"

void readInput(double *numbers, char *operators)
{
    for (int i = 0; i < NUMBERS_AMOUNT + OPERATORS_AMOUNT; i++) {
        if (i % 2 == 0) 
            scanf("%lf", &numbers[i / 2]);
        else
            scanf(" %c", &operators[i / 2]);
    }
    return;
}

void createRanges(double *numbers, doubleRange *ranges)
{
    for (int i = 0; i < NUMBERS_AMOUNT; i++) {
        ranges[i].min = numbers[i];
        ranges[i].max = numbers[i];
    }
    return;
}