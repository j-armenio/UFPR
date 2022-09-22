#include <stdio.h>
#include <stdlib.h>
#include "libaleat.h"

int ALEAT(int min, int max){
    int r;

    r = (rand() % (max + 1 - min)) + min;

    return r;
}