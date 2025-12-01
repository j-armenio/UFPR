# Programação Paralela

## Tipos de computadores paralelos:

1. **Multicomputadores: Cluster de Máquinas (Memória Distribuída)**
Conjunto de computadores completos e independentes, interligados por rede.
Cada nó possui sua própria memória local e independente.
Comunicação entre processos feita por troca de mensagens, utilizando MPI (Message Passing Interface).

2. **Multiprocessadores (Memória Compartilhada)**
Todos processadores residem em uma única máquina e compartilham o mesmo espaço de endereçamento de memória física.
A comunicação entre os processos/threads é feita por meio de variáveis compartilhadas, geralmente usando threads (POSIX Threads ou OpenMP).

- NUMA (Non-Uniform Memory Access): A memória é dividida em módulos e cada módulo está associado a um/grupo de processadores. Existe uma interconexão entre esses espaços de memória dos processadores, que é muito mais rápida que trocar por mensagens na rede.
- UMA (Uniform Memory Access): Todos processadores tem acesso uniforme a qualquer posição da memória compartilhada.

## Modelo PRAM
