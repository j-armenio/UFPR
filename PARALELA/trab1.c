#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_THREADS 64
#define N_TIMES 1000

int n_threads;                  // número efetivo de threads
int n_total_elements;           // número total de elementos
float *vector;                  // vetor para os calculos
float partial_sum[MAX_THREADS]; //
pthread_barrier_t barrier;      // barreira

void *thread_work(void *arg)
{
    int tid = *(int *)arg;

    while (1) {
        long base  = n_total_elements / n_threads;
        long rem   = n_total_elements % n_threads;
        long start = tid * base + (tid < rem ? tid : rem);
        long len   = base + (tid < rem ? 1 : 0);
        long end   = start + len;

        // barreira para sincronizar inicio da rodada
        pthread_barrier_wait(&barrier);

        // calculo da soma parcial
        float my_partial = 0;

        long i = start;
        for (; i + 3 < end; i += 4)
            my_partial += vector[i] + vector[i+1] + vector[i+2] + vector[i+3];
        for (; i < end; i++)
            my_partial += vector[i];
        
        partial_sum[tid] = my_partial;

        // barreira para sincronizar fim dos calculos partial_sum
        pthread_barrier_wait(&barrier);

        // calcular prefixo acumulado
        float my_prefix_sum = 0;
        for (int j=0; j < tid; j++)
            my_prefix_sum += partial_sum[j];
        
        // sobreescrever inplace
        float acc = my_prefix_sum;

        i = start;
        for (; i + 3 < end; i += 4) {
            acc += vector[i];
            vector[i] = acc;
            acc += vector[i+1];
            vector[i+1] = acc;
            acc += vector[i+2];
            vector[i+2] = acc;
            acc += vector[i+3];
            vector[i+3] = acc;
        }
        for (; i < end; i++) {
            acc += vector[i];
            vector[i] = acc;
        }

        // barreira para sincronizar fim da escrita de todos
        pthread_barrier_wait(&barrier);

        // thread 0 retorna ao fim da rodada, as demais ficam vivas esperando
        if (tid == 0)
            return NULL;
    }
}

void parallelPrefixSumPth(void)
{
    static int initialized = 0;
    static pthread_t threads[MAX_THREADS];
    static int tid[MAX_THREADS]; // thread id

    if (!initialized ) {
        // inicializa barreira para n_threads
        pthread_barrier_init(&barrier, NULL, n_threads);

        for (int i=1; i < n_threads; i++) {
            tid[i] = i;
            pthread_create(&threads[i], NULL, thread_work, &tid[i]);
        }

        tid[0] = 0;
        initialized = 1;
    }

    thread_work(&tid[0]);
}

int main(int argc, char **argv) 
{
    n_total_elements = atoi(argv[1]);
    n_threads = atoi(argv[2]);

    // aloca vetor original, não será alterado
    float *init_vector = (float *) malloc(n_total_elements * sizeof(float));
    if (init_vector == NULL) {
        printf("Erro ao alocar vetor inicial de %d elementos\n", n_total_elements);
        return 1;
    }        

    // aloca o vetor que vai ser processado pelas threads
    vector = (float *) malloc(n_total_elements * sizeof(float));
    if (vector == NULL) {
        printf("Erro ao alocar vetor de %d elementos\n", n_total_elements);
        return 1;
    }        

    // preenche o vetor inicial com valores aleatorios
    int r;
    for (long i=0; i < n_total_elements; i++) {
        r = rand();
        init_vector[i] = (r % 10);
    }

    // processa algoritmo N_TIMES
    for (int i=0; i < N_TIMES; i++) {
        memcpy((float *)vector, (float *)init_vector, n_total_elements * sizeof(float));

        // chama algoritmo de processamento paralelo N_TIMES
        parallelPrefixSumPth();
    }

    return 0;
}