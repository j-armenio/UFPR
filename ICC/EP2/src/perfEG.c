#include <stdio.h>
#include <stdlib.h>

// Resolver o sistema Ax = b pelas substituições sucessivas
// { ordem, matriz triangular superior, vetor independente } -> vetor resultado
double *retrosub(int n, double **A, double *b)
{

}

int main()
{
    double **A, *b, *x;
    int n;

    if (scanf("%d", &n) != 1) {
        printf("Erro ao ler a ordem da matriz\n");
        exit(1);
    }

    A = (double **) malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A[i] = (double *) malloc(n * sizeof(double));
    }
    b = (double *) malloc(n * sizeof(double));
    x = (double *) malloc(n * sizeof(double));
 
    // Ler o sistema
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scanf("%lf", &A[i][j]) != 1) {
                printf("Erro ao ler a matriz\n");
                exit(1);
            }
        }
        if (scanf("%lf", &b[i]) != 1) {
            printf("Erro ao ler o vetor independente\n");
            exit(1);
        }
    }
    printf("\n");

    printf("A = \n");
    for (int i = 0; i < n; i++) {
        printf("[ ");
        for (int j = 0; j < n; j++) {
            printf("%.2lf ", A[i][j]);
        }
        printf("]\n");
        printf("[ %.2lf ]\n", b[i]);
    }
}