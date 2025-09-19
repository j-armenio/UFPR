#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *printHello(void *threadid) 
{ // função a ser executada quando uma thread eh criada
    long id;
    id = (long)threadid; // converte void para long
    printf("Thread %ld: Olá mundo\n", id);
    pthread_exit(NULL); // finaliza thread
}

int main() {
    pthread_t t1, t2;
    int create; // var que recebe o retorno da p_create
    long num;   // identificador da thread
    num = 1;
    printf("Main: criando a thread %ld\n", num);
    create = pthread_create(&t1, NULL, printHello, (void *)num);

    num = 2;
    printf("Main: criando a thread %ld\n", num);
    create = pthread_create(&t2, NULL, printHello, (void *)num);

    printf("Main: finalizando\n");
    return 0;
}