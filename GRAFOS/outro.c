#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "grafo.h"

#define MAX_LINHA 2048

// Definição das estruturas internas
typedef struct aresta {
    char* destino;
    int peso;
    struct aresta* prox;
} aresta;

typedef struct vertice {
    char* nome;
    aresta* arestas;
    struct vertice* prox;
    int visitado; // Para DFS e BFS
    int cor;      // Para bipartido (0 ou 1)
    int distancia;// Para diametro
} vertice;

struct grafo {
    char* nome;
    vertice* vertices;
};

// Função auxiliar para buscar um vértice pelo nome
static vertice* buscar_vertice(vertice* lista, const char* nome) {
    for (; lista; lista = lista->prox) {
        if (strcmp(lista->nome, nome) == 0) return lista;
    }
    return NULL;
}

// Função auxiliar para criar um novo vértice e adicioná-lo à lista
static vertice* adicionar_vertice(vertice** lista, const char* nome) {
    vertice* existente = buscar_vertice(*lista, nome);
    if (existente) return existente;

    vertice* novo = malloc(sizeof(vertice));
    novo->nome = strdup(nome);
    novo->arestas = NULL;
    novo->prox = *lista;
    novo->visitado = 0;
    novo->cor = -1;
    novo->distancia = -1;
    *lista = novo;
    return novo;
}

// Função auxiliar para adicionar uma aresta
static void adicionar_aresta(vertice* origem, const char* destino, int peso) {
    aresta* nova = malloc(sizeof(aresta));
    nova->destino = strdup(destino);
    nova->peso = peso;
    nova->prox = origem->arestas;
    origem->arestas = nova;
}

// Implementação de le_grafo
grafo* le_grafo(FILE* f) {
    if (!f) return NULL;

    grafo* g = malloc(sizeof(grafo));
    g->vertices = NULL;
    char linha[MAX_LINHA];

    do {
        if (!fgets(linha, MAX_LINHA, f)) return NULL;
    } while (linha[0] == '\n' || strncmp(linha, "//", 2) == 0);

    linha[strcspn(linha, "\n")] = 0;
    g->nome = strdup(linha);

    while (fgets(linha, MAX_LINHA, f)) {
        char* p = linha;
        while (isspace(*p)) p++;
        if (*p == '\0' || *p == '\n' || strncmp(p, "//", 2) == 0) continue;

        linha[strcspn(linha, "\n")] = 0;
        char v1[1024], v2[1024];
        int peso = 1;

        if (sscanf(p, "%1023s -- %1023s %d", v1, v2, &peso) == 3 ||
            sscanf(p, "%1023s -- %1023s", v1, v2) == 2) {
            vertice* a = adicionar_vertice(&g->vertices, v1);
            adicionar_vertice(&g->vertices, v2);
            adicionar_aresta(a, v2, peso);
        } else {
            char nome[1024];
            if (sscanf(p, "%1023s", nome) == 1) {
                adicionar_vertice(&g->vertices, nome);
            }
        }
    }

    return g;
}

unsigned int destroi_grafo(grafo* g) {
    if (!g) return 0;
    vertice* v = g->vertices;
    while (v) {
        vertice* tempv = v;
        aresta* a = v->arestas;
        while (a) {
            aresta* tempa = a;
            free(a->destino);
            a = a->prox;
            free(tempa);
        }
        free(v->nome);
        v = v->prox;
        free(tempv);
    }
    free(g->nome);
    free(g);
    return 1;
}

char* nome(grafo* g) {
    return g ? g->nome : NULL;
}

unsigned int n_vertices(grafo* g) {
    unsigned int count = 0;
    for (vertice* v = g->vertices; v; v = v->prox) count++;
    return count;
}

unsigned int n_arestas(grafo* g) {
    unsigned int count = 0;
    for (vertice* v = g->vertices; v; v = v->prox)
        for (aresta* a = v->arestas; a; a = a->prox) count++;
    return count;
}

static void dfs(vertice* v, grafo* g) {
    if (v->visitado) return;
    v->visitado = 1;
    for (aresta* a = v->arestas; a; a = a->prox) {
        vertice* viz = buscar_vertice(g->vertices, a->destino);
        if (viz && !viz->visitado) dfs(viz, g);
    }
}

unsigned int n_componentes(grafo* g) {
    unsigned int count = 0;
    for (vertice* v = g->vertices; v; v = v->prox) v->visitado = 0;
    for (vertice* v = g->vertices; v; v = v->prox)
        if (!v->visitado) { dfs(v, g); count++; }
    return count;
}

unsigned int bipartido(grafo* g) {
    for (vertice* v = g->vertices; v; v = v->prox) {
        v->visitado = 0;
        v->cor = -1;
    }
    for (vertice* v = g->vertices; v; v = v->prox) {
        if (!v->visitado) {
            v->cor = 0;
            v->visitado = 1;
            vertice* fila[1000];
            int ini = 0, fim = 0;
            fila[fim++] = v;
            while (ini < fim) {
                vertice* atual = fila[ini++];
                for (aresta* a = atual->arestas; a; a = a->prox) {
                    vertice* viz = buscar_vertice(g->vertices, a->destino);
                    if (!viz->visitado) {
                        viz->visitado = 1;
                        viz->cor = 1 - atual->cor;
                        fila[fim++] = viz;
                    } else if (viz->cor == atual->cor) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

// Retorna uma string com o diametro convertido
char* diametros(grafo* g) {
    unsigned int diam = 0;

    // BFS para cada vértice
    for (vertice* v = g->vertices; v; v = v->prox) {
        for (vertice* u = g->vertices; u; u = u->prox) {
            u->visitado = 0;
            u->distancia = -1;
        }
        vertice* fila[1000];
        int ini = 0, fim = 0;
        v->visitado = 1;
        v->distancia = 0;
        fila[fim++] = v;

        while (ini < fim) {
            vertice* atual = fila[ini++];
            for (aresta* a = atual->arestas; a; a = a->prox) {
                vertice* viz = buscar_vertice(g->vertices, a->destino);
                if (!viz->visitado) {
                    viz->visitado = 1;
                    viz->distancia = atual->distancia + 1;
                    if ((unsigned int)viz->distancia > diam)
		    	diam = (unsigned int)viz->distancia;

                    fila[fim++] = viz;
                }
            }
        }
    }

    // Converte para string
    char* s = malloc(32);
    snprintf(s, 32, "%u", diam);
    return s;
}
