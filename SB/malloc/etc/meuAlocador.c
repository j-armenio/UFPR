#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "meuAlocador.h"

// Bloco:
// | estado(8 bytes) | tamanho(8 bytes) | dados(até 4096 bytes) |

void *topoInicialHeap = NULL;
void *aux = NULL;

// Armazena o endereço do topo corrente da heap e armazena em topoInicialHeap
void iniciaAlocador()
{
    topoInicialHeap = sbrk(0);
    return;
}

// Restaura o valor original da heap
void finalizaAlocador()
{
    if (brk(topoInicialHeap) == 0)
        return;
    else
        printf("Erro ao restaurar heap.\n");
        return;
    return;
}

// Aloca um bloco com num_bytes na heap e retorna o endereço inicial desse bloco
void *alocaMem(int num_bytes)
{
    if (num_bytes <= 0) {
        printf("Número de bytes inválido.\n");
        return NULL;
    }

    void *posAtualHeap = topoInicialHeap;
    void *topoAtualHeap = sbrk(0);

    void *blocoLivre = NULL;
    int64_t menorTamanho = __INT64_MAX__;

    int64_t estado, tamanho;

    // (best-fit) Percorre toda a heap e seleciona o nó com menor bloco, que é maior do que o solicitado
    while (posAtualHeap < topoAtualHeap) { // Começa da posição atual da Heap até seu inicio
        estado = *(int64_t*)posAtualHeap;
        tamanho = *(int64_t*)(posAtualHeap+8);

        if (estado == 0 && tamanho >= (num_bytes+16) && tamanho < menorTamanho) {
            blocoLivre = posAtualHeap;
            menorTamanho = tamanho;
        }

        posAtualHeap += (tamanho/4096 + 1 + 16);
    }

    // se encontrou um bloco adequado
    if (blocoLivre) {
        *(int64_t*)blocoLivre = 1; // Marca 'estado' como ocupado
        return blocoLivre + 16;
    }
    else { // Não encontrou um bloco adequado
        int64_t novoTamanho = (((num_bytes) + 4095) / 4096) * 4096; // Ajuste para múltiplos de 4096 bytes
        void *novoBloco = sbrk(novoTamanho + 16);

        if (novoBloco == (void*)-1) {
            printf("Erro ao alocar bloco.\n");
            return NULL;
        }

        *(int64_t*)novoBloco = 1;
        *((int64_t*)(novoBloco+8)) = num_bytes;
        
        return novoBloco + 16;
    }
}

// Devolve para heap o bloco alocado
int liberaMem(void *bloco)
{
    if (!bloco) {
        return -1;
    }

    void *inicioBloco = bloco-16;
    int64_t estado = *(int64_t*)inicioBloco;

    if (estado == 0) {
        return -2;
    }

    *(int64_t*)inicioBloco = 0; // Marca como livre

    return 0;
}

// Imprime um mapa da memória da região da heap
void imprimeMapa()
{
    void *topoHeap = sbrk(0);
    void *atual = topoInicialHeap;

    printf("Mapa da heap:\n");

    while (atual < topoHeap) {
        int64_t estado = *(int64_t*)atual;
        int64_t tamanho = *((int64_t*)(atual + 8));

        printf("[");
        for (int i = 0; i < 16; i++) {
            printf("#"); // Representa os metadados
        }
        for (int i = 0; i < tamanho; i++) {
            printf(estado ? "+" : "-"); // + para ocupado, - para livre
        }
        printf("]\n");

        atual +=  (((tamanho) + 4095) / 4096) * 4096 + 16; // Move para o próximo bloco
    }
    printf("\n");
}

int main (long int argc, char** argv) {
    void *a, *b, *c;

    iniciaAlocador();               // Impressão esperada
    // imprimeMapa();                  // <vazio>

    a = (void *) alocaMem(8200);
    // imprimeMapa();                  // ################**********
    b = (void *) alocaMem(4);
    // imprimeMapa();                  // ################**********##############****
    aux = sbrk(0);
    liberaMem(a);
    // imprimeMapa();                  // ################----------##############****
    aux = sbrk(0);
    
    c = (void*)alocaMem(4000);
    
    liberaMem(b);                   // ################----------------------------
    aux = sbrk(0);

    liberaMem(c);                   // ################----------------------------
    aux = sbrk(0);
                                    // ou
                                    // <vazio>
    finalizaAlocador();
}