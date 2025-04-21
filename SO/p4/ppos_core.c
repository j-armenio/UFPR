#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "ppos_data.h"
#include "ppos.h"
#include "queue.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, "PPOS: " fmt, ##__VA_ARGS__)
#else 
#define DEBUG_PRINT(fmt, ...)
#endif

/* =========== VARIAVEIS GLOBAIS =========== */

long long int task_counter; // contador de id de tarefa
int user_tasks;             // contador de tarefas ativas do usuário

task_t main_task;           // tarefa do contexto main
task_t dispatcher_task;     // tarefa do dispachante
task_t *current_task;       // tarefa atual
task_t *ready_queue;        // fila de tarefas prontas

/* =========== FUNCOES AUXILIARES =========== */

// Implementação de impressão para biblioteca de fila
void print_task(void *ptr) 
{
    task_t *elem = ptr;

    if (!elem)
        return;
    
    elem->prev ? printf ("%d", elem->prev->id) : printf ("*") ;
    printf("<%d>", elem->id);
    elem->next ? printf("%d", elem->next->id) : printf("*") ;
}

/* =========== FUNCOES DE DESPACHAMENTO E ESCALONAMENTO =========== */

// Coloca tarefa atual no fim da fila de prontas e devolve CPU ao dispatcher
void task_yield()
{
    DEBUG_PRINT("yield na task %d\n", current_task->id);

    queue_append((queue_t **) &ready_queue, (queue_t *) current_task);
    user_tasks++;

    current_task->status = READY;
    
    dispatcher_task.status = RUNNING;
    task_switch(&dispatcher_task);
}

// Varre a fila e pega a primeira task, depois pega a próxima primeira e assim vai indo
task_t *scheduler()
{
    if (!ready_queue){
        printf("Fila de tarefas prontas vazia\n");
        return NULL;
    }

    task_t *ret_task = ready_queue;
    task_t *aux = ready_queue;
            
    // escolhe a tarefa com a menor prioridade dinamica
    do {
        if (
            (aux->prio_d < ret_task->prio_d) || 
            ((aux->prio_d == ret_task->prio_d) && (aux->prio_e < ret_task->prio_e))  // criterio de desempate usa prio_e
        )
            ret_task = aux;

        aux = aux->next;
    } while (aux != ready_queue);

    DEBUG_PRINT("tarefa escolhida da fila de prontas: %d, prio_e: %d, prio:_d: %d\n", ret_task->id, ret_task->prio_e, ret_task->prio_d);

    #ifdef DEBUG // impressao da fila de prontos e suas prioridades
    queue_print("PPOS: READY", (queue_t *) ready_queue, print_task);


    if (ready_queue == NULL)
        printf("[]\n");
    printf("PPOS: PRIO: [");
    aux = ready_queue;
    do {
        printf("%d(%d,%d)", aux->id, aux->prio_e, aux->prio_d);
        aux = aux->next;
        if (aux != ready_queue)
            printf(" "); // separador
    } while (aux != ready_queue);
    printf("]\n");
    #endif

    // "Aging" adicionar -1 em todas tarefas da fila de prontas
    aux = ready_queue;
    do {
        if (aux != ret_task)
            aux->prio_d += ALPHA;
            
        if (aux->prio_d < PRIO_MIN)
            aux->prio_d = PRIO_MIN;
        else if (aux->prio_d > PRIO_MAX)
            aux->prio_d = PRIO_MAX;

        aux = aux->next;
    } while (aux != ready_queue);

    if (ret_task != NULL) 
        ret_task->prio_d = ret_task->prio_e; // rejuvenescimento

    return ret_task;
}

// Determina qual a proxima tarefa a executar a cada troca de contexto
void dispatcher()
{   
    DEBUG_PRINT("dispatcher no controle\n");
    dispatcher_task.status = RUNNING;

    // retira o dispatcher da fila de prontas, para evitar que ele ative a si proprio
    queue_remove((queue_t **) &ready_queue, (queue_t *) &dispatcher_task);
    user_tasks--;

    // enquanto houveren tarefas de usuário
    while ( user_tasks > 0 ) {
        DEBUG_PRINT("tasks de usuario restantes: %d\n", user_tasks);

        // escolher a próxima tarefa a executar
        task_t *next_task = scheduler();

        // escalonador escolheu uma tarefa?
        if ( next_task ) {
            dispatcher_task.status = SUSPENDED;

            queue_remove((queue_t **) &ready_queue, (queue_t *) next_task);
            user_tasks--;

            // transfere o controle para a próxima tarefa
            task_switch(next_task);

            switch (next_task->status)
            {
            case READY:
                break;
            
            case FINISHED:
                // aqui é seguro liberar a stack, pois ela nao está mais em execucao
                free(next_task->context.uc_stack.ss_sp);
                VALGRIND_STACK_DEREGISTER(next_task->context.uc_stack.ss_sp); // valgrind config
                next_task->context.uc_stack.ss_sp = NULL;
                next_task = NULL;
                break;

            case SUSPENDED:
                break;

            case RUNNING:
                break;

            default:
                printf("task em estado estranho: %d\n", next_task->status);
                break;
            }
        }
    }

    // encera a tarefa dispatcher
    task_exit(0);
}

/* =========== FUNCOES GERAIS =========== */

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void ppos_init()
{ 
    DEBUG_PRINT("iniciando...\n");

    // desativa o buffer da saida padrao (stdout), usado pela função printf
    setvbuf(stdout, 0, _IONBF, 0);

    // configura variáveis globais de sistema
    task_counter = 0;
    user_tasks = 0;
    ready_queue = NULL;

    // configura task main
    if (getcontext(&main_task.context) == -1){
        perror("Erro ao salvar o contexto atual\n");
        return;
    }
    main_task.id = task_counter++;
    main_task.prev = main_task.next = NULL;
    main_task.status = RUNNING;
    main_task.prio_e = main_task.prio_d = 0; // prioridade padrao

    current_task = &main_task;

    // inicializa dispatcher
    task_init(&dispatcher_task, dispatcher, NULL);

    DEBUG_PRINT("main task id %d, dispatcher task id %d\n", main_task.id, dispatcher_task.id);
}

/* =========== GERENCIAMENTO DE TAREFAS =========== */

// Ajusta a prioridade estática da tarefa task para o valor prio (que deve estar entre -20 e +20). Caso task seja nulo, ajusta a prioridade da tarefa atual. 
void task_setprio (task_t *task, int prio)
{
    if (prio > 20 || prio < -20) {
        DEBUG_PRINT("prioridade fora do limite aceitavel\n");
        return;
    }  

    if (task == NULL) {
        DEBUG_PRINT("task nula, ajustando task corrente\n");
        task = current_task;
    } 

    task->prio_e = task->prio_d = prio;
    DEBUG_PRINT("ajustando task %d para prio %d\n", task->id, prio);
}

// Devolve o valor da prioridade estática da tarefa task (ou da tarefa corrente, se task for nulo).
int task_getprio (task_t *task) 
{
    if (task == NULL)
        return current_task->prio_e;
    return task->prio_e;
}

// Inicializa uma nova tarefa. Retorna um ID> 0 ou erro.
int task_init (task_t *task,			// descritor da nova tarefa
               void  (*start_func)(void *),	// funcao corpo da tarefa
               void   *arg)			// argumentos para a tarefa
{
    if (task == NULL || start_func == NULL) {
        perror("Parametro de criação de task nulo\n");
        return -1;
    }

    // salva contexto
    if (getcontext(&task->context) == -1){
        perror("Erro ao salvar o contexto atual\n");
        return -1;
    }

    // aloca memória para pilha do contexto
    char *stack = malloc(STACKSIZE);
    if (!stack) {
        perror("Erro ao alocar pilha de contexto\n");
        return -1;
    }

    // inicializa variaveis de contexto
    task->context.uc_stack.ss_sp = stack;
    task->context.uc_stack.ss_size = STACKSIZE;
    task->context.uc_stack.ss_flags = 0;
    task->context.uc_link = 0;
    
    makecontext(&task->context, (void*)(*start_func), 1, arg);

    task->id = task_counter++;
    task->prev = task->next = NULL;
    task->prio_e = task->prio_d = 0; // prioridade padrão

    task->vg_id = VALGRIND_STACK_REGISTER(task->context.uc_stack.ss_sp, task->context.uc_stack.ss_sp + STACKSIZE); // valgrind config

    DEBUG_PRINT("task %d criada pela task %d\n", task->id, current_task->id);

    task->status = READY;
    // adiciona task no fim da fila de tarefas prontas

    queue_append((queue_t **) &ready_queue, (queue_t *) task);
    user_tasks++;

    DEBUG_PRINT("task %d adicionada a fila de prontos\n", task->id);
    #ifdef DEBUG
    queue_print("PPOS: READY", (queue_t *) ready_queue, print_task);
    #endif

    return task->id;
}

// Termina a tarefa corrente com um status de encerramento
void task_exit(int exit_code)
{
    DEBUG_PRINT("exiting task %d\n", current_task->id);

    switch (exit_code)
    {
    case 0: 
        if (current_task == &dispatcher_task) { // liberar dispatcher no fim do programa
            free(((&dispatcher_task)->context.uc_stack.ss_sp));
            VALGRIND_STACK_DEREGISTER(&dispatcher_task.context.uc_stack.ss_sp); // valgrind config
            dispatcher_task.context.uc_stack.ss_sp = NULL;

            DEBUG_PRINT("liberando PPOS dispatcher\n");

            exit(exit_code);
        } 
        else // finaliza task e passa controle para o dispatcher
        {
            current_task->status = FINISHED;
            task_switch(&dispatcher_task);
            break;
        }
    
    default:
        break;
    }
}

// Alterna a execução para a tarefa indicada
int task_switch(task_t *task)
{
    if (!task) {
        perror("tentando trocar para task null\n");
        return -1;
    }

    DEBUG_PRINT("switch task %d -> task %d\n", current_task->id, task->id);

    task->status = RUNNING;

    task_t *aux = current_task;
    current_task = task;
    if (swapcontext(&aux->context, &task->context) == -1){
        perror("Erro ao trocar de contexto\n");
        return -1;
    }

    return 0;
}

// Retorna o identificador da tarefa corrente (main deve ser 0)
int task_id() { return current_task->id; }
