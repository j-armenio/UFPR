#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define EPOCHS 5 // ele converge em 5 epocas
#define INPUT_SIZE 2
#define NUM_SAMPLES 4

double vector_dot(double *a, double *b, int n);
int step_function(double n);

// ============= PERCEPTRON E SUAS FUNCOES =============

struct Perceptron {
    double *weights; // bias = weights[0]
};

struct Perceptron *get_perceptron(int input_size) 
{
    struct Perceptron *p = malloc(sizeof(struct Perceptron));
    p->weights = malloc(sizeof(double) * (input_size+1));

    // inicializa o bias e os pesos como 0
    for (int i=0; i < input_size+1; i++)
        p->weights[i] = 0;

    return p;
}

// Treinamento ajustando os pesos com base no dataset X e labels t
void fit(struct Perceptron *p, double X[NUM_SAMPLES][INPUT_SIZE], int *t)
{
    for (int e = 0; e < EPOCHS; e++) 
    {
        // Passa sample por sample
        for (int i=0; i < NUM_SAMPLES; i++) {
            // sum = bias + (pesos * entradas)
            // weights[0] é o bias, weights[1..n] são os pesos das entradas
            double sum = p->weights[0] + vector_dot(&p->weights[1], X[i], INPUT_SIZE);

            // predicao com função de ativacao
            int pred = step_function(sum);

            int error = t[i] - pred;
            if (error != 0) {
                // atualiza os pesos
                for (int j=0; j < INPUT_SIZE; j++) 
                    p->weights[j+1] += error * X[i][j];
                // atualiza o bias
                p->weights[0] += error;
            }
        }
    }
}

// ============= FUNCOES AUXILIARES =============

// Função de ativação
int step_function(double n) { return n >= 0.0f ? 1 : 0; }

// Produto interno de vetores
double vector_dot(double* a, double* b, int n)
{
    double sum = 0.0;
    for (int i=0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

// ============= FUNCAO MAIN =============

int main()
{
    // DATASET
    // X[sample][feature]
    double X[4][2] = {
        { 2.0, 2.0 },
        { -2.0, -2.0 },
        { -2.0, 2.0 },
        { -1.0, 1.0 },
    };
    // labels
    int t[4] = { 0, 1, 0, 1};

    struct Perceptron *p = get_perceptron(INPUT_SIZE);

     // Treinamento
    fit(p, X, t);

    printf("Pesos finais: Bias: %.2f, W1: %.2f, W2: %.2f\n",
                                p->weights[0], p->weights[1], p->weights[2]);

    // Libera memória
    free(p->weights);
    free(p);
}