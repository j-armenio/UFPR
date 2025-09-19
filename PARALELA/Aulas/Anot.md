# Programação Paralela

## Processos x Threads

- Se implementadas corretamente, threads tem vantagem sobre processos pois os fork()(forma de criar processos novos) tem mais overhead que threads.
- Ao utilizar fork() é criada uma segunda cópia do processo pai e ambos são independentes, cada um com seu endereço e suas cópias de variáveis.
- Threads compartilham um mesmo espaço de endereçamento, economizando CPU. Tem comunicação mais simples, pois compartilham espaço de memória, enquanto fork() precisa de técnicas como pipe(|).

## Pthreads

Biblioteca que implementa o **POSIX**, padrão para criar e manipular threads.

`#include <pthread.h>`

### Pthreads API

Tem subrotinas divididas em 4 grupos principais:

1. **Gerenciamento de thread**: criar, remover, definir e entre outros;
2. **Mutexes**: rotinas que lidam com exclusão mutua, permitem criar, destruir, bloquear e desbloquear mutexes;
3. **Variáveis de condição**: endereçam comunicações entre threads que compartilham um mutex, baseado em condições especificadas pelo programador. Inclui funções para criar, destruir, aguardar e sinalizar com base nos valores das variáveis especificadas;
4. **Sincronização**: gerenciamento de bloqueios e barreiras de leitura e escrita;

Por padrão, usa `pthread_`.

### Compilação

`gcc codico.c -o codigo -lpthread`

### Gerenciamento de Thread

#### Criação de Thread

O fluxo principal (main) é por padrão uma thread, todas outras devem ser criadas explicitamente.

pthread\_create cria uma nova thread.

`pthread_create(thread, attr, start_routine, arg)`
* thread: identificador único para uma nova thread
* attr: especificar atributos de uma nova thread, NULL para valores padrão.
* start\_routine: rotina que thread executará depois de ser criada.
* arg: argumento único passado para start\_routine, deve ser passado por referência como um ponteiro de void.

Uma vez criadas, threads podem criar outras threads e não há hierarquia implícita ou dependência entre as threads.

#### Atributos de Thread

Por padrão, são criadas com determinados atributos, alguns dos quais podem ser alterados. Ex: escopo, escalonamento, endereço da pilha, uso da pilha, prioridade.

#### Finalização da Thread

Formas de finalizar uma thread:

* Quando a thread termina a execução.
* Quando a thread faz a chamada `pthread_exit`, retornando um status para a thread pai, independente da conclusão ou não do trabalho.
* Quando é cancelado por outra thread usando `pthread_cancel`.

`pthread_exit()` permite especificar o termino de uma thread e caso a thread finalize normalmente, é dispensável. Resaltar que não fecha arquivos abertos.

#### Join e Detaching

**join** é uma das maneiras de realizar sincronização de threads. `pthread_join` faz que a thread que chamou esperar até que a thread especificada termine.

`pthread_join (threadid, status)`

A rotina `pthread_detach()` serve para desanexar uma thread da outra de forma explicita.

#### Passagem de argumentos para threads

create permite a passagem de argumentos. Para passar mais de um argumento, deve-se utilizar uma struct e passar a mesma como ponteiro em void, por referência.

#### Exemplo de criação e finalização de Thread

Cria 2 threads e cada uma imprimirá "hello world" e será finalizada.

!!! ver exemplo1.c !!!

####
















