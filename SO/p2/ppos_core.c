#include <stdio.h>
#include "ppos.h"
#include "ppos_data.h"

#define DEBUG

long long int task_counter = 0;
task_t *mainTask;
task_t *curTask;

// Inicializa estruturas internas do SO
void ppos_init()
{
    /* desativa o buffer da saida padrao (stdout), usado pela função printf */
    setvbuf(stdout, 0, _IONBF, 0);

    getcontext(&mainTask->context);

    return;
}

// Inicia uma nova tarefa
int task_init(task_t *task, void (*start_routine)(void*), void *arg)
{
    // Salva o contexto atual em task e o configura
    if (getcontext(&task->context) == -1) {
        printf("PPOS: Erro ao salvar o contexto atual\n");
        return -1;
    }
    task_counter++;
    task->tid = task_counter;
    task->status = "READY";

    #ifdef DEBUG
    printf("PPOS: task %d created by task %d\n", task->tid, curTask->tid);
    #endif

    makecontext(task, start_routine, arg);
    return 0;
}

int task_switch(task_t *task)
{
    task_t *prevTask;
    if (swapcontext(&prevTask->context, &task->context) == -1) {
        printf("PPOS: Erro ao trocar de contexto.\n");
        return -1; 
    }
    curTask = task;

    #ifdef DEBUG
    printf("PPOS: switch task %d -> task %d", curTask->tid, task->tid);
    #endif

    return 0;
}

void task_exit(int exit_code)
{

}

int task_id()
{

}