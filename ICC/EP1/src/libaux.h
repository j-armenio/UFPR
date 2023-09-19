#ifndef _LIB_AUX_H_
#define _LIB_AUX_H_

#define NUMBERS_AMOUNT 5
#define OPERATORS_AMOUNT 4

typedef struct doubleRange {
    double min;
    double max;
} doubleRange;

void readInput(double *numbers, char *operators);   
void createRanges(double *numbers, doubleRange *ranges);
void printRanges(doubleRange *ranges);
void solveOperations(doubleRange *ranges, char *operators);

#endif