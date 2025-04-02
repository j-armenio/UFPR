#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "ppos_data.h"
#include "ppos.h"

#define DEBUG

long long int task_counter;
task_t main_task;
task_t *current_task;

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void ppos_init()
{
    #ifdef DEBUG 
    printf("PPOS: init\n");
    #endif

    // desativa o buffer da saida padrao (stdout), usado pela função printf
    setvbuf(stdout, 0, _IONBF, 0);

    task_counter=0;

    // Cria a task principal e direciona current_task para ela
    char *stack = malloc(STACKSIZE);
    if (stack) { // configura stack para o contexto
        main_task.context.uc_stack.ss_sp = stack;
        main_task.context.uc_stack.ss_size = STACKSIZE;
        main_task.context.uc_stack.ss_flags = 0;
        main_task.context.uc_link = 0;
    } else {
        perror("PPOS: Erro na criação da pilha de contexto");
        return;
    }

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
    // aloca memória para pilha do contexto
    char *stack = malloc(STACKSIZE);
    if (stack) { // configura stack para o contexto
        task->context.uc_stack.ss_sp = stack;
        task->context.uc_stack.ss_size = STACKSIZE;
        task->context.uc_stack.ss_flags = 0;
        task->context.uc_link = 0;
    } else {
        perror("PPOS: Erro na criação da pilha de contexto");
        return -1;
    }

    if (getcontext(&task->context) == -1){
        perror("PPOS: Erro ao salvar o contexto atual\n");
        return -1;
    }

    task->id = task_counter++;
    task->prev = task->next = NULL;

    makecontext(&task->context, (void*)(*start_func), 1, arg);

    task->status = 0;

    #ifdef DEBUG
    printf("PPOS: task %d created by task %d\n", task->id, current_task->id);
    #endif

    printf("task %d stack location: %p\n", task->id, stack);

    return task->id;
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

    return 0;
}

// Termina a tarefa corrente com um status de encerramento
void task_exit(int exit_code)
{
    #ifdef DEBUG
    printf("PPOS: exiting task %d\n", current_task->id);
    #endif

    if (current_task->id != 0){ // Não liberar stack da main_task
        free(current_task->context.uc_stack.ss_sp);
    }
    task_switch(&main_task);
}

// retorna o identificador da tarefa corrente (main deve ser 0)
int task_id()
{
    return current_task->id;
}

