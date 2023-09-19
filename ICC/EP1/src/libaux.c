#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <fenv.h>
#include <math.h>

#include "libaux.h"

void sumRanges(doubleRange num1, doubleRange num2)
{
    
}

void subtractRanges(doubleRange num1, doubleRange num2)
{
    
}

void multiplyRanges(doubleRange num1, doubleRange num2)
{
    
}

void divideRanges(doubleRange num1, doubleRange num2)
{
    
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
        if (fabs(numbers[i]) == 0){
            ranges[i].min = -0;
            ranges[i].max = +0;
        } else {
            // Acha o maior num menor que x 
            fesetround(FE_DOWNWARD);
            ranges[i].min = nextafter(numbers[i], -DBL_MAX);
            
            // Acha o menor num maior que x
            fesetround(FE_UPWARD);
            ranges[i].max = nextafter(numbers[i], DBL_MAX);
        }
    }
    return;
}

void printRanges(doubleRange *ranges)
{
    for (int i = 0; i < NUMBERS_AMOUNT; i++)
        printf("Range %d: [%1.8e, %1.8e]\n", i, ranges[i].min, ranges[i].max);
    return;
}

void solveOperations(doubleRange *ranges, char *operators)
{
    doubleRange currentValue = ranges[0];

    for (int i = 0; i < OPERATORS_AMOUNT; i++)
    {
        switch (operators[i])
        {
        case '+':
            sumRanges(currentValue, ranges[i+1]);
            break;
        case '-':
            subtractRanges(currentValue, ranges[i+1]);
            break;
        case '*':
            multiplyRanges(currentValue, ranges[i+1]);
            break;
        case '/':
            divideRanges(currentValue, ranges[i+1]);
            break;
        default:
            print("Operação Inválida.\n");
            return;
            break;
        }
    }
}
