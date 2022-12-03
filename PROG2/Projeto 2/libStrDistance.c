#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libStrDistance.h"

// Return the min number between three integers
int min(int a, int b, int c) {	

    int res;
	if(a <= b && a <= c) 
		res = a;
	else if(b <= a && b <= c)
		res = b;
	else if(c <= a && c <= b) 
		res = c;
    return res;
}


// Returns a int that means the distance between two strings
int levenshtein(char *s1, char *s2) {

    unsigned int x, y, s1len, s2len = 0;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;

    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;
    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;
    for (x = 1; x <= s2len; x++)
        for (y = 1; y <= s1len; y++)
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));

    return(matrix[s2len][s1len]);
}