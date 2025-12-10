# Prova Programação Paralela

# Pthreads
`#include <pthread.h>`  

**Gerenciamento de Thread**  
`pthread_t thread;`
- `int pthread_create(pthread_t *thread, pthread_attr_t *attr, *(void *)*start_routine, void *arg);` Cria uma nova thread.
- `void pthread_exit(void *retval);` Finaliza uma thread, retornando um status para pai.
- `int pthread_cancel(pthread_t thread);` Uma thread cancela outra.
- `int pthread_join(pthread_t thread, void **retval);` A thread que chama espera até que a thread especificada termine.
- `int pthread_detach(pthread_t thread);` Desanexa um thread de outra.

**Mutexes** e **Variáveis de Condição**  
fds  

**Barreiras**  
`pthread_barrier_t barrier;`
- `int pthread_barrier_init(pthread_barrier_t *barrier, pthread_barrierattr_t *attr, unsigned count_threads);` Inicializa uma barreira com n threads.
- `int pthread_barrier_wait(pthread_barrier_t *barrier);` Quando uma thread atinge a barreira, elas espera todas as outras a atingirem.

# MPI
`#include <mpi.h>`

- `MPI_Init(int *argc, char **argv);` e `MPI_Finalize();` Inicializa e finaliza o ambiente de execução MPI.
- `MPI_COMM_WORLD` Comunicador universal com todos processos em execução.
- `MPI_Comm_rank(MPI_Comm comm, int *rank);` Retorna em rank a posição do processo em comm.
- `MPI_Comm_size();` Retorna o total de processos em comm.

**Comunicação um-pra-um**
- `MPI_Send(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);` Envio padrão de mensagens.
- `MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);` Recepção padrão de mensagens.

**Comunicação coletiva**  
Todos processos devem chamar a mesma rotina.
- `MPI_Bcast(void *buf, int count, MPI_Datatype, int root, MPI_Comm comm);` Broadcast, faz uma mensagem de um processo chegar a todos processos no comm.
- `MPI_Reduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_OP op, int root, MPI_Comm comm);` Reduce, reune tem vários subtotais espalhados nos processos e acumula calculando op em um único processo no comm.
- `MPI_Scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);` Scatter, divide em partes iguais os dados de uma mensagem e distribui ordenadamente cada uma das partes por cada um dos processos do comm..
- `MPI_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm);` Gather, recolhe ordenadamente num único processo um conjunto de mensagens de todos processos do comm.

# OpenMP
`#include <omp.h>`

```c
#pragma omp parallel
{
    // código nesse bloco será executado em paralelo
    int id = omp_get_thread_num();  // retorna id da thread
    int n  = omp_get_num_threads(); // retorna número de threads sendo usadas
}

// ambos seguintes definem número de threads como 4
omp_set_num_threads(4);
#pragma omp parallel num_threads(4) {}
```

**For loop**
```c
// divide as iterações entre as threads automaticamente
#pragma omp parallel
{
    #pragma omp for
    for (int i=0; i < N; i++) {
        a[i] = b[i] + c[i];
    }
}
```

**Redução**
```c
// executa a redução sem ter condição de corrida em 'soma'
int soma =0;

#pragma omp parallel for reduction(+:soma)
for (int i=0; i < N; i++) {
    soma += v[i];
}
```

**Controle de acesso**
```c
#pragma omp parallel
{
    int id = omp_get_thread_num();
    #pragma omp critical
    {
        // sessão crítica, apenas uma thread executa essa região de cada vez
    }
}

#pragma omp parallel for
for (int i=0; i < N; i++) {
    #pragma omp atomic // mais leve, para operações com memória
    soma += v[i]
}

#pragma omp parallel
{
    do_stuff();

    #pragma omp barrier // threads esperam todas chegarem aqui

    do_more_stuff();
}
```

**Outras diretivas**
```c
#pragma omp parallel
{
    #pragma omp single
    {
        // bloco executado por apenas uma thread, as outras esperam
        // barreira implicita
    }
}

#pragma omp parallel sections 
{   // divide em blocos independentes, cada um em uma thread
    #pragma omp section
    {
        task_A();
    }
    #pragma omp section
    {
        task_B();
    }
    #pragma omp section
    {
        task_C();
    }
}
```