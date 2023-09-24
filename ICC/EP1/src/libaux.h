#ifndef _LIB_AUX_H_
#define _LIB_AUX_H_

#define NUMBERS_AMOUNT 5
#define OPERATORS_AMOUNT 4

typedef struct floatRange {
    float min;
    float max;
} floatRange;

void readInput(float *numbers, char *operators);   
void createRanges(float *numbers, floatRange *ranges);
void printRanges(floatRange *ranges);
void solveOperations(floatRange *ranges, char *operators);

void testFunction(floatRange *ranges, char *operators);

#endif