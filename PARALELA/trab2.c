#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define Q_POINTS_AMT 128
#define P_POINTS_AMT 400000
#define DIMENSIONS 300
#define K_NEIGHBORS 1024

int main(int argc, char **argv) 
{
    MPI_Init(&argc, &argv);

    int rank, n_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

    // valores default
    int nq = Q_POINTS_AMT; // número de pontos em Q
    int npp = P_POINTS_AMT;  // número de pontos em P
    int D = DIMENSIONS;    // número de dimensões
    int k = K_NEIGHBORS;   // tamanho do conj de vizinhos

    int params[4];
    params[0] = nq; params[1] = npp; params[2] = D; params[3] = k;

    float *Q = NULL;
    float *P = NULL;

    P = (float *)malloc((size_t)npp * D * sizeof(float));

    if (rank == 0) { // somente rank 0 deve gerar os conjuntos Q e P
        Q = (float *)malloc((size_t)nq * D * sizeof(float));

        for (int i=0; i < n; i++) {
            for (int j=0; j < d; j++) {
                P[i*D + j] = (float)rand() / (float)RAND_MAX;
            }
        }
        for (int i=0; i < nq; i++) {
            for (int j=0; j < d; j++) {
                Q[i*D + j] = (float)rand() / (float)RAND_MAX;
            }
        }
    }

    // manda P para todos processos
    MPI_Bcast(P, (int)((size_t)n * D), MPI_FLOAT, 0, MPI_COMM_WORLD);

    // root ve quantidade de Q que cada proc vai receber
    int *pts_amount = NULL;
    int *pts_offset = NULL;

    if (rank == 0) {
        pts_amount = (int*)malloc(n_procs * sizeof(int));
        pts_offset = (int*)malloc(n_procs * sizeof(int));

        int base = nq / n_procs;
        int rem = nq % n_procs;
        int offset = 0;

        for (int p=0; p < n_procs; p++) {
            pts_amount[p] = base + (p < rem ? 1 : 0);
            pts_offset[p] = offset;
            offset += pts_amount[p];
        }
    }

    // quantidade de pontos de Q que cada proc recebe
    int local_nq = 0;

    // pontos de Q q vai receber
    float *local_Q = (float*)malloc((size_t)local_nq * D * sizeof(float));

    if (rank == 0) {
        int *pts_amount_f = (int *)malloc(nprocs * sizeof(int));
        int *pts_offset_f = (int *)malloc(nprocs * sizeof(int));

        // converte os pontos para float, para saber o tamanho que tem q mandar
        for (int p=0; p < nprocs; ++p) {
            pts_amount_f[p] = pts_amount[p] * D;
            pts_offset_f[p] = pts_offset[p] * D;
        }

        MPI_Scatterv(Q, pts_amount_f, pts_offset_f, MPI_FLOAT,
                    local_Q, local_nq * D, MPI_FLOAT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatterv(NULL, NULL, NULL, MPI_FLOAT,
                    local_Q, local_nq * D, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

    // execução do knn
    

    MPI_Finalize();
    return 0;
}