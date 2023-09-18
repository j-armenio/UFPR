#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <fenv.h>
#include <math.h>

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
    fesetround(FE_TONEAREST); // Configura arredondamento para mais próximo

    for (int i = 0; i < NUMBERS_AMOUNT; i++) {
        ranges[i].min = nextafter(numbers[i], -DBL_MAX);
        ranges[i].max = nextafter(numbers[i], DBL_MAX);
    }
    return;
}

void printRanges(doubleRange *ranges)
{
    for (int i = 0; i < NUMBERS_AMOUNT; i++)
        printf("Range %d: [%lf, %lf]\n", i, ranges[i].min, ranges[i].max);
    return;
}