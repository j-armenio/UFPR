// Estruturas de dados internas do sistema operacional

#ifndef __PPOS_DATA__
#define __PPOS_DATA__

#include <ucontext.h>		// biblioteca POSIX de trocas de contexto
#include <valgrind/valgrind.h>


#define STACKSIZE 64*1024  // Tamanho da stack de cada tarefa
#define ALPHA -1           // Fator de envelhecimento de tarefas
#define PRIO_MIN -20       // Prioridade maxima (escala negativa)
#define PRIO_MAX 20        // Prioridade minima (escala negativa)
#define QUANTUM 20          // Quantum de tempo para cada tarefa

// Macros para status da task
#define READY     0
#define RUNNING   1
#define SUSPENDED 2
#define FINISHED  3

// Macros para tipo de task
#define USER      0
#define KERNEL    1
#define DISPATCHER 2

// Estrutura que define um Task Control Block (TCB)
typedef struct task_t
{
  struct task_t *prev, *next ; 	// ponteiros para usar em filas
  int id ;				       // identificador da tarefa
  ucontext_t context ;	 // contexto armazenado da tarefa
  short status ;			   // pronta, rodando, suspensa, ...
  short prio_e ;         // prioridade estatica
  short prio_d ;         // prioridade dinamica
  short task_type ;      // tipo da tarefa (user, kernel, dispatcher)

  int vg_id; // valgrind config
} task_t ;

// estrutura que define um semáforo
typedef struct
{
  // preencher quando necessário
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
  // preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif