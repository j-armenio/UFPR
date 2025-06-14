#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "grafo.h"

typedef struct aresta {
    char *nome_u;
    char *nome_v;
} aresta;

typedef struct vizinho {
    int v;
    int peso;
    struct vizinho* prox;
} vizinho;

typedef struct {
    char* nome;
    vizinho* adj;
} vertice_grafo;

struct grafo {
    char* nome;
    vertice_grafo** vertices;
    unsigned int n;
};

// Função para mapear nome do vértice para índice
static int indice_vertice(grafo* g, const char* nome) {
    for (unsigned int i = 0; i < g->n; i++) {
        if (strcmp(g->vertices[i]->nome, nome) == 0) return (int)i;
    }
    return -1;
}

// Adiciona um vértice, se não existir
static int adicionar_vertice(grafo* g, const char* nome) {
    int idx = indice_vertice(g, nome);
    if (idx != -1) return idx;

    g->vertices = realloc(g->vertices, (g->n + 1) * sizeof(vertice_grafo*));
    g->vertices[g->n] = malloc(sizeof(vertice_grafo));
    g->vertices[g->n]->nome = strdup(nome);
    g->vertices[g->n]->adj = NULL;
    return (int)g->n++;
}

// Adiciona uma aresta entre dois vértices
static void adicionar_aresta(grafo* g, const char* orig, const char* dest, int peso) {
    int u = adicionar_vertice(g, orig);
    int v = adicionar_vertice(g, dest);

    vizinho* novo = malloc(sizeof(vizinho));
    novo->v = v;
    novo->peso = peso;
    novo->prox = g->vertices[u]->adj;
    g->vertices[u]->adj = novo;
}

// Lê grafo do arquivo
struct grafo* le_grafo(FILE* f) {
    if (!f) return NULL;
    grafo* g = malloc(sizeof(grafo));
    g->vertices = NULL;
    g->n = 0;

    char buffer[2048];
    do {
        if (!fgets(buffer, sizeof(buffer), f)) return NULL;
    } while (buffer[0] == '\n' || strncmp(buffer, "//", 2) == 0);
    buffer[strcspn(buffer, "\n")] = 0;
    g->nome = strdup(buffer);

    while (fgets(buffer, sizeof(buffer), f)) {
        char* p = buffer;
        while (isspace(*p)) p++;
        if (*p == '\0' || *p == '\n' || strncmp(p, "//", 2) == 0) continue;

        buffer[strcspn(buffer, "\n")] = 0;
        char v1[1024], v2[1024];
        int peso;

        if (sscanf(p, "%1023s -- %1023s %d", v1, v2, &peso) == 3 ||
            sscanf(p, "%1023s -- %1023s", v1, v2) == 2) {
            if (sscanf(p, "%*s -- %*s %d", &peso) != 1) peso = 1;
            adicionar_aresta(g, v1, v2, peso);
            adicionar_aresta(g, v2, v1, peso); // grafo não direcionado
        } else if (sscanf(p, "%1023s", v1) == 1) {
            adicionar_vertice(g, v1);
        }
    }

    return g;
}

unsigned int destroi_grafo(grafo* g) {
    if (!g) return 0;
    for (unsigned int i = 0; i < g->n; i++) {
        vizinho* v = g->vertices[i]->adj;
        while (v) {
            vizinho* tmp = v;
            v = v->prox;
            free(tmp);
        }
        free(g->vertices[i]->nome);
        free(g->vertices[i]);
    }
    free(g->vertices);
    free(g->nome);
    free(g);
    return 1;
}

char* nome(grafo* g) {
    return g ? g->nome : NULL;
}

unsigned int n_vertices(grafo* g) {
    return g ? g->n : 0;
}

unsigned int n_arestas(grafo* g) {
    unsigned int total = 0;
    for (unsigned int i = 0; i < g->n; i++) {
        for (vizinho* v = g->vertices[i]->adj; v; v = v->prox) total++;
    }
    return total / 2; // não direcionado
}

// Marca um componente a partir de um vértice usando BFS
static void comp(grafo* g, int r, int* estado, int* componente, int c) {
    int* fila = malloc(sizeof(int) * g->n);
    int ini = 0, fim = 0;

    estado[r] = 1;
    componente[r] = c;
    fila[fim++] = r;

    while (ini < fim) {
        int v = fila[ini++];
        for (vizinho* w = g->vertices[v]->adj; w != NULL; w = w->prox) {
            if (estado[w->v] == 0) {
                estado[w->v] = 1;
                componente[w->v] = c;
                fila[fim++] = w->v;
            }
        }
        estado[v] = 2;
    }

    free(fila);
}

unsigned int n_componentes(grafo* g) {
    int* estado = calloc(g->n, sizeof(int));
    int* componente = calloc(g->n, sizeof(int));
    int c = 0;

    for (unsigned int v = 0; v < g->n; v++) {
        if (estado[v] == 0) {
            componente[v] = c;
            comp(g, (int)v, estado, componente, c);
            c++;
        }
    }

    free(estado);
    free(componente);
    return (unsigned int) c;
}

unsigned int bipartido(grafo* g) {
    int* cor = malloc(sizeof(int) * g->n);
    for (unsigned int i = 0; i < g->n; i++) cor[i] = -1;

    for (unsigned int s = 0; s < g->n; s++) {
        if (cor[s] == -1) {
            int* fila = malloc(sizeof(int) * g->n);
            int ini = 0, fim = 0;
            cor[s] = 0;
            fila[fim++] = (int)s;

            while (ini < fim) {
                int v = fila[ini++];
                for (vizinho* w = g->vertices[v]->adj; w; w = w->prox) {
                    if (cor[w->v] == -1) {
                        cor[w->v] = 1 - cor[v];
                        fila[fim++] = w->v;
                    } else if (cor[w->v] == cor[v]) {
                        free(fila);
                        free(cor);
                        return 0; // ciclo ímpar
                    }
                }
            }
            free(fila);
        }
    }

    free(cor);
    return 1;
}

char* diametros(grafo* g) {
    int* estado = calloc(g->n, sizeof(int));
    int* componente = calloc(g->n, sizeof(int));
    int* fila = malloc(sizeof(int) * g->n);
    int* dist = malloc(sizeof(int) * g->n);
    unsigned int* resultado = NULL;
    int c = 0;

    for (unsigned int v = 0; v < g->n; v++) {
        if (estado[v] != 0) continue;

        int ini = 0, fim = 0;
        fila[fim++] = (int)v;
        estado[v] = 1;
        componente[v] = c;

        while (ini < fim) {
            int u = fila[ini++];
            for (vizinho* w = g->vertices[u]->adj; w; w = w->prox) {
                if (estado[w->v] == 0) {
                    estado[w->v] = 1;
                    componente[w->v] = c;
                    fila[fim++] = w->v;
                }
            }
        }

        unsigned int diametro = 0;
        for (unsigned int i = 0; i < g->n; i++) {
            if (componente[i] != c) continue;
            for (unsigned int j = 0; j < g->n; j++) dist[j] = -1;

            ini = fim = 0;
            fila[fim++] = (int)i;
            dist[i] = 0;

            while (ini < fim) {
                int u = fila[ini++];
                for (vizinho* w = g->vertices[u]->adj; w; w = w->prox) {
                    if (dist[w->v] == -1) {
                        dist[w->v] = dist[u] + w->peso;
                        fila[fim++] = w->v;
                        if ((unsigned int)dist[w->v] > diametro) {
                            diametro = (unsigned int)dist[w->v];
                        }
                    }
                }
            }
        }

        resultado = realloc(resultado, sizeof(unsigned int) * (unsigned int)(c + 1));
        resultado[c++] = diametro;
    }

    // ordena os resultados
    for (int i = 0; i < c - 1; i++) {
        for (int j = i + 1; j < c; j++) {
            if (resultado[i] > resultado[j]) {
                unsigned int tmp = resultado[i];
                resultado[i] = resultado[j];
                resultado[j] = tmp;
            }
        }
    }

    // converte para string
    size_t tam = (unsigned int)16 * (unsigned int)c;
    char* saida = malloc(tam);
    saida[0] = '\0';
    for (int i = 0; i < c; i++) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%u%s", resultado[i], (i < c - 1 ? " " : ""));
        strncat(saida, buffer, tam - strlen(saida) - 1);
    }

    free(estado);
    free(componente);
    free(fila);
    free(dist);
    free(resultado);
    return saida;
}

// ================ // 

static int compara_strings(const void *a, const void *b) 
{ 
    char *const *s1 = a;
    char *const *s2 = b;
    return strcmp(*s1, *s2);
}

static void dfs_vertices_corte(
    grafo *g, 
    int r, 
    int *pre, 
    int *low, 
    int *pai, 
    int *corte, 
    int *tempo)
{
    int filhos = 0;
    pre[r] = low[r] = ++(*tempo);

    // percorre vizinhança de r
    for (vizinho *v_vizinho = g->vertices[r]->adj; v_vizinho != NULL; v_vizinho = v_vizinho->prox) {
        int v = v_vizinho->v;

        if (pre[v] == -1) { // vertice ainda nao visitado
            filhos++;
            pai[v] = r;
            dfs_vertices_corte(g, v, pre, low, pai, corte, tempo);

            // atualiza low de r com ancestral mais alto
            low[r] = (low[r] < low[v]) ? low[r] : low[v];

            // eh vertice de corte se:
            // eh raiz com mais de um filho OU 
            // nao eh raiz e low de filho eh maior ou igual
            if ((pai[r] == -1 && filhos > 1) || (pai[r] != -1 && low[v] >= pre[r]))
                corte[r] = 1;
        }

        else if (v != pai[r]) // vertice ja visitado (menos pai)
            low[r] = (low[r] < pre[v]) ? low[r] : pre[v];
    }
}

// Retorna um array ordenado com os vertices de corte
char *vertices_corte(grafo *g) 
{
    if (!g || g->n == 0) return strdup("");

    int tempo = 0;
    unsigned int n = g->n;
    int *pre = malloc(n * sizeof(int));
    int *low = malloc(n * sizeof(int));
    int *pai = malloc(n * sizeof(int));
    int *corte = calloc(n, sizeof(int));

    for (unsigned int i=0; i < n; i++) {
        pre[i] = -1; // nao visitado
        pai[i] = -1;
    }

    for (unsigned int i=0; i < n; i++) {
        if (pre[i] == -1)
            dfs_vertices_corte(g, (int)i, pre, low, pai, corte, &tempo);
    }

    // formata saida
    char **nomes_corte = NULL;
    size_t n_corte = 0;
    size_t tam_string = 1; // \0

    for (unsigned int i=0; i < n; i++) {
        if (corte[i]) {
            n_corte++;
            nomes_corte = realloc(nomes_corte, n_corte * sizeof(char*));
            nomes_corte[n_corte - 1] = g->vertices[i]->nome;
            tam_string += strlen(g->vertices[i]->nome) + 1; // +1 por causa do espaço ' '
        }
    }

    // ordena em ordem alfabetica
    if (n_corte > 0)
        qsort(nomes_corte, n_corte, sizeof(char *), compara_strings);

    // faz string final
    char *str_final = malloc(tam_string);
    str_final[0] = '\0';

    for (size_t i=0; i < n_corte; i++) {
        strcat(str_final, nomes_corte[i]);
        if (i < n_corte - 1)
            strcat(str_final, " ");
    }

    free(pre);
    free(low);
    free(pai);
    free(corte);
    free(nomes_corte);

    return str_final;
}

static int compara_arestas(const void *a, const void *b)
{
    const aresta *aresta_a = (const aresta *)a;
    const aresta *aresta_b = (const aresta *)b;

    int cmp = strcmp(aresta_a->nome_u, aresta_b->nome_u);
    if (cmp != 0) 
        return cmp;
    return strcmp(aresta_a->nome_v, aresta_b->nome_v);
}

static void dfs_arestas_corte(
    grafo *g, 
    int r, 
    int *pre, 
    int *low, 
    int *pai, 
    int *tempo, 
    aresta **pontes, 
    size_t *n_pontes) 
{
    pre[r] = low[r] = ++(*tempo);

    for (vizinho *v_vizinho = g->vertices[r]->adj; v_vizinho != NULL; v_vizinho = v_vizinho->prox) {
        int v = v_vizinho->v;

        if (v == pai[r]) // ignora pai
            continue;

        if (pre[v] == -1) {
            pai[v] = r;
            dfs_arestas_corte(g, v, pre, low, pai, tempo, pontes, n_pontes);

            low[r] = (low[r] < low[v]) ? low[r] : low[v];

            if (low[v] > pre[r]) { // condição de ponte
                (*n_pontes)++;
                *pontes = realloc(*pontes, (*n_pontes) * sizeof(aresta));
                (*pontes)[*n_pontes - 1] = (aresta){g->vertices[r]->nome, g->vertices[v]->nome};
            }
        } else { // aresta de retorno
            low[r] = (low[r] < pre[v]) ? low[r] : pre[v];
        }
    }
}

// Retorna um array ordenado com as arestas de corte
char *arestas_corte(grafo *g) 
{
    if (!g || g->n < 2) return strdup("");

    int tempo = 0;
    unsigned int n = g->n;
    int *pre = malloc(n * sizeof(int));
    int *low = malloc(n * sizeof(int));
    int *pai = malloc(n * sizeof(int));

    aresta *pontes = NULL;
    size_t n_pontes = 0;

    for (unsigned int i=0; i < n; i++) {
        pre[i] = -1;
        pai[i] = -1;
    }

    for (unsigned int i=0; i < n; i++) {
        if (pre[i] == -1)
            dfs_arestas_corte(g, (int)i, pre, low, pai, &tempo, &pontes, &n_pontes);
    }

    // ordena u e v da aresta
    for (size_t i=0; i < n_pontes; i++) {
        if (strcmp(pontes[i].nome_u, pontes[i].nome_v) > 0) {
            char *temp = pontes[i].nome_u;
            pontes[i].nome_u = pontes[i].nome_v;
            pontes[i].nome_v = temp;
        }
    }

    // ordena lista de arestas
    if (n_pontes > 0)
        qsort(pontes, n_pontes, sizeof(aresta), compara_arestas);

    // formata string
    size_t tam_string = 1;
    for (size_t i=0; i < n_pontes; i++) {
        tam_string += strlen(pontes[i].nome_u) + 1;
        tam_string += strlen(pontes[i].nome_v) + 1; 
    }

    char *str_final = malloc(tam_string);
    if (!str_final) return NULL;

    str_final[0] = '\0';

    for (size_t i=0; i < n_pontes; i++) {
        strcat(str_final, pontes[i].nome_u);
        strcat(str_final, " ");
        strcat(str_final, pontes[i].nome_v);
        if (i < n_pontes - 1) {
            strcat(str_final, " ");
        }
    }   

    free(pre);
    free(low);
    free(pai);
    free(pontes);

    return str_final;
}
 