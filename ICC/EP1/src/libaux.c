#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <fenv.h>
#include <math.h>

#include "libaux.h"

// Acha o maior num menor que x 
float biggestUnder(float num)
{
    if (fabs(num) == 0)
        return -0;

    fesetround(FE_DOWNWARD);
    return nextafter(num, -DBL_MAX);
}

// Acha o menor num maior que x
float smallestAbove(float num)
{
    if (fabs(num) == 0)
        return +0;

    fesetround(FE_UPWARD);
    return nextafter(num, DBL_MAX);
}

void printRanges(floatRange *ranges)
{
    for (int i = 0; i < NUMBERS_AMOUNT; i++)
        printf("Range %d: [%1.8e, %1.8e]\n", i, ranges[i].min, ranges[i].max);
    return;
}

void printRange(floatRange range)
{
    printf("[%1.8e, %1.8e]", range.min, range.max);
    return;
}

void printError(floatRange range)
{
    float error;
    
    // Erro absoluto
    error = range.max - range.min;
    printf("EA: %1.8e; ", error);

    // Erro relativo
    error = (range.max - range.min) / range.min;
    printf("ER: %1.8e; ", error);

    // ULPs
    if (range.max == range.min)
        printf("ULPs: 0 ");
    else {
        int32_t max = *(int32_t *)&range.max;
        int32_t min = *(int32_t *)&range.min;
        int64_t diff = max - min;
        if (diff < 0)
            diff = -diff;
        printf("ULPs: %ld ", diff);
    }

    return;
}

floatRange sumRanges(floatRange num1, floatRange num2)
{
    floatRange ret;

    ret.min = biggestUnder(num1.min + num2.min);
    ret.max = smallestAbove(num1.max + num2.max);

    return ret;
}

floatRange subtractRanges(floatRange num1, floatRange num2)
{
    floatRange ret;

    ret.min = biggestUnder(num1.min - num2.min);
    ret.max = smallestAbove(num1.max - num2.max);

    return ret;
}

floatRange multiplyRanges(floatRange num1, floatRange num2)
{
    floatRange ret;

    ret.min = biggestUnder(num1.min * num2.min);
    ret.max = smallestAbove(num1.max * num2.max);

    return ret;
}

floatRange divideRanges(floatRange num1, floatRange num2)
{
    floatRange ret;

    if (fabs(num2.min) == 0 || fabs(num2.max) == 0) {
        ret.min = -INFINITY;
        ret.max = INFINITY;
    } else {
        ret.min = biggestUnder(num1.min / num2.min);
        ret.max = smallestAbove(num1.max / num2.max);
    }

    return ret;
}

/* --------------------------------------------------------------- */

void readInput(float *numbers, char *operators)
{
    for (int i = 0; i < NUMBERS_AMOUNT + OPERATORS_AMOUNT; i++) {
        if (i % 2 == 0) 
            scanf("%f", &numbers[i / 2]);
        else
            scanf(" %c", &operators[i / 2]);
    }
    return;
}

void createRanges(float *numbers, floatRange *ranges)
{
    for (int i = 0; i < NUMBERS_AMOUNT; i++) 
    {
        // Acha o maior num menor que x 
        ranges[i].min = biggestUnder(numbers[i]);
        // Acha o menor num maior que x
        ranges[i].max = smallestAbove(numbers[i]);
    }
    return;
}

void solveOperations(floatRange *ranges, char *operators)
{
    floatRange currentValue = ranges[0];

    for (int i = 0; i < OPERATORS_AMOUNT; i++)
    {
        printf("%d:\n", i+1);

        switch (operators[i])
        {
        case '+':
            printf("[%1.8e,%1.8e] + [%1.8e,%1.8e] =\n", currentValue.min, currentValue.max, ranges[i+1].min, ranges[i+1].max);
            currentValue = sumRanges(currentValue, ranges[i+1]);
            printf("[%1.8e,%1.8e]\n", currentValue.min, currentValue.max);
            printError(currentValue);
            printf("\n");
            break;
        case '-':
            printf("[%1.8e,%1.8e] - [%1.8e,%1.8e] =\n", currentValue.min, currentValue.max, ranges[i+1].min, ranges[i+1].max);
            currentValue = subtractRanges(currentValue, ranges[i+1]);
            printf("[%1.8e,%1.8e]\n", currentValue.min, currentValue.max);
            printError(currentValue);
            printf("\n");
            break;
        case '*':
            printf("[%1.8e,%1.8e] * [%1.8e,%1.8e] =\n", currentValue.min, currentValue.max, ranges[i+1].min, ranges[i+1].max);
            currentValue = multiplyRanges(currentValue, ranges[i+1]);
            printf("[%1.8e,%1.8e]\n", currentValue.min, currentValue.max);
            printError(currentValue);
            printf("\n");
            break;
        case '/':
            printf("[%1.8e,%1.8e] / [%1.8e,%1.8e] =\n", currentValue.min, currentValue.max, ranges[i+1].min, ranges[i+1].max);
            currentValue = divideRanges(currentValue, ranges[i+1]);
            printf("[%1.8e,%1.8e]\n", currentValue.min, currentValue.max);
            printError(currentValue);
            printf("\n");
            break;
        default:
            printf("Operação Inválida.\n");
            return;
            break;
        }
        printf("\n");
    }
}

void testFunction(floatRange *ranges, char *operators)
{
    floatRange currentValue = ranges[0];

    currentValue = multiplyRanges(ranges[0], ranges[1]);
    printRange(currentValue);
    
    printError(currentValue);
    printf("\n");

    currentValue = sumRanges(currentValue, ranges[2]);
    printRange(currentValue);
    printf("\n");
    printError(currentValue);
    printf("\n");

    currentValue = subtractRanges(currentValue, ranges[3]);
    printRange(currentValue);
    printf("\n");
    printError(currentValue);
    printf("\n");

    currentValue = divideRanges(currentValue, ranges[4]);
    printRange(currentValue);
    printf("\n");
    printError(currentValue);
    printf("\n");

    return;
}