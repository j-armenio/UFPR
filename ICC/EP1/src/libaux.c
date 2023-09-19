#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <fenv.h>
#include <math.h>

#include "libaux.h"

// Acha o maior num menor que x 
double biggestUnder(double num)
{
    if (fabs(num) == 0)
        return -0;

    fesetround(FE_DOWNWARD);
    return nextafter(num, -DBL_MAX);
}

// Acha o menor num maior que x
double smallestAbove(double num)
{
    if (fabs(num) == 0)
        return +0;

    fesetround(FE_UPWARD);
    return nextafter(num, DBL_MAX);
}

doubleRange sumRanges(doubleRange num1, doubleRange num2)
{
    doubleRange ret;

    ret.min = biggestUnder(num1.min + num2.min);
    ret.max = smallestAbove(num1.max + num2.max);

    return ret;
}

doubleRange subtractRanges(doubleRange num1, doubleRange num2)
{
    doubleRange ret;

    ret.min = biggestUnder(num1.min - num2.min);
    ret.max = smallestAbove(num1.max - num2.max);

    return ret;
}

doubleRange multiplyRanges(doubleRange num1, doubleRange num2)
{
    doubleRange ret;

    return ret;
}

doubleRange divideRanges(doubleRange num1, doubleRange num2)
{
    doubleRange ret;

    return ret;
}

/* --------------------------------------------------------------- */

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
    for (int i = 0; i < NUMBERS_AMOUNT; i++) 
    {
        // Acha o maior num menor que x 
        ranges[i].max = biggestUnder(numbers[i]);
        // Acha o menor num maior que x
        ranges[i].max = smallestAbove(numbers[i]);
    }
    return;
}

void printRanges(doubleRange *ranges)
{
    for (int i = 0; i < NUMBERS_AMOUNT; i++)
        printf("Range %d: [%1.8e, %1.8e]\n", i, ranges[i].min, ranges[i].max);
    return;
}

void printError(doubleRange range)
{
    
    

    printf("")
}

void solveOperations(doubleRange *ranges, char *operators)
{
    doubleRange currentValue = ranges[0];

    for (int i = 0; i < OPERATORS_AMOUNT; i++)
    {
        printf("%d:\n", i);

        switch (operators[i])
        {
        case '+':
            printf("[%1.8e,%1.8e] + [%1.8e,%1.8e] =\n", currentValue.min, currentValue.max, ranges[i+1].min, ranges[i+1].max);
            currentValue = sumRanges(currentValue, ranges[i+1]);
            printf("[%1.8e,%1.8e]\n", currentValue.min, currentValue.max);
            printError(currentValue);
            break;
        case '-':
            printf("[%1.8e,%1.8e] - [%1.8e,%1.8e] =\n", currentValue.min, currentValue.max, ranges[i+1].min, ranges[i+1].max);
            currentValue = subtractRanges(currentValue, ranges[i+1]);
            printf("[%1.8e,%1.8e]\n", currentValue.min, currentValue.max);
            printError(currentValue);
            break;
        case '*':
            printf("[%1.8e,%1.8e] * [%1.8e,%1.8e] =\n", currentValue.min, currentValue.max, ranges[i+1].min, ranges[i+1].max);
            currentValue = multiplyRanges(currentValue, ranges[i+1]);
            printf("[%1.8e,%1.8e]\n", currentValue.min, currentValue.max);
            printError(currentValue);
            break;
        case '/':
            printf("[%1.8e,%1.8e] / [%1.8e,%1.8e] =\n", currentValue.min, currentValue.max, ranges[i+1].min, ranges[i+1].max);
            currentValue = divideRanges(currentValue, ranges[i+1]);
            printf("[%1.8e,%1.8e]\n", currentValue.min, currentValue.max);
            printError(currentValue);
            break;
        default:
            printf("Operação Inválida.\n");
            return;
            break;
        }
    }
}
