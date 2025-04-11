#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "ppos_data.h"
#include "ppos.h"

// #define DEBUG

long long int task_counter;
task_t main_task;
task_t *current_task;
task_t *finished_task;

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void ppos_init()
{
    #ifdef DEBUG 
    printf("PPOS: init\n");
    #endif

    // desativa o buffer da saida padrao (stdout), usado pela função printf
    setvbuf(stdout, 0, _IONBF, 0);

    task_counter=0;

    if (getcontext(&main_task.context) == -1){
        perror("PPOS: Erro ao salvar o contexto atual\n");
        return;
    }

    main_task.id = task_counter++;
    main_task.prev = main_task.next = NULL;

    // makecontext(&main_task.context, NULL, 1, NULL);

    main_task.status = 0;

    current_task = &main_task;
}

// Inicializa uma nova tarefa. Retorna um ID> 0 ou erro.
int task_init (task_t *task,			// descritor da nova tarefa
               void  (*start_func)(void *),	// funcao corpo da tarefa
               void   *arg)			// argumentos para a tarefa
{
    if (getcontext(&task->context) == -1){
        perror("PPOS: Erro ao salvar o contexto atual\n");
        return -1;
    }

    // aloca memória para pilha do contexto
    char *stack = malloc(STACKSIZE);
    if (!stack) {
        perror("PPOS: Erro ao alocar pilha de contexto\n");
    }

    if (stack) { // configura stack para o contexto
        task->context.uc_stack.ss_sp = stack;
        task->context.uc_stack.ss_size = STACKSIZE;
        task->context.uc_stack.ss_flags = 0;
        task->context.uc_link = 0;
    } else {
        perror("PPOS: Erro na criação da pilha de contexto\n");
        return -1;
    }

    task->id = task_counter++;
    task->prev = task->next = NULL;

    makecontext(&task->context, (void*)(*start_func), 1, arg);

    task->status = 0;

    #ifdef DEBUG
    printf("PPOS: task %d created by task %d\n", task->id, current_task->id);
    #endif

    return task->id;
}

// Termina a tarefa corrente com um status de encerramento
void task_exit(int exit_code)
{
    #ifdef DEBUG
    printf("PPOS: exiting task %d\n", current_task->id);
    #endif

    finished_task = current_task;

    task_switch(&main_task);
}

// Alterna a execução para a tarefa indicada
int task_switch(task_t *task)
{   
    #ifdef DEBUG
    printf("PPOS: switch task %d -> task %d\n", current_task->id, task->id);
    #endif

    task_t *aux = current_task;
    current_task = task;

    if (swapcontext(&aux->context, &task->context) == -1){
        perror("PPOS: Erro ao trocar de contexto\n");
        return -1;
    }

    // Aqui é seguro liberar a stack, pois ela não está mais em execução
    if (finished_task != NULL) {
        free(finished_task->context.uc_stack.ss_sp);
        finished_task->context.uc_stack.ss_sp = NULL;
        finished_task = NULL;
    }

    return 0;
}

// retorna o identificador da tarefa corrente (main deve ser 0)
int task_id() { return current_task->id; }
