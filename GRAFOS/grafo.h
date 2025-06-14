#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// estrutura de dados para representar um grafo

typedef struct grafo grafo;

//------------------------------------------------------------------------------
// lê um grafo de f e o devolve
//
// um grafo é representado num arquivo por uma "string" que é o nome
// do grafo seguido de uma lista de vértices e arestas
//
// o nome do grafo, cada vértice e cada aresta aparecem numa linha por si só
// cada linha tem no máximo 2047 caracteres
// linhas iniciando por // são consideradas comentários e são ignoradas
//
// um vértice é representado por uma "string"
//
// uma aresta é representada por uma linha
// xxx -- yyy ppp
//
// onde xxx e yyy são nomes de vértices e ppp (opcional) é um int não negativo indicando o peso da aresta
//
// se um vértice faz parte de uma aresta, não é necessário nomeá-lo individualmente em uma linha
//
// a função supõe que a entrada está corretamente construída e não faz nenhuma checagem 
// caso a entrada não esteja corretamente construída, o comportamento da função é indefinido
//
// abaixo, a título de exemplo, a representação de um grafo com quatro
// vértices e dois componentes, um K_3 e um K_1
/*

// o nome do grafo
triângulo_com_vértice

// uma lista com três arestas e seus pesos
um -- dois 12
dois -- quatro 24
quatro -- um 41

// um vértice isolado
três

*/

grafo *le_grafo(FILE *f);

//------------------------------------------------------------------------------
// desaloca toda a estrutura de dados alocada em g
//
// devolve 1 em caso de sucesso e 0 em caso de erro

unsigned int destroi_grafo(grafo *g);

//------------------------------------------------------------------------------
// devolve o nome de g

char *nome(grafo *g);

//------------------------------------------------------------------------------
// devolve 1 se g é bipartido e 0 caso contrário

unsigned int bipartido(grafo *g);

//------------------------------------------------------------------------------
// devolve o número de vértices em g

unsigned int n_vertices(grafo *g);

//------------------------------------------------------------------------------
// devolve o número de arestas em g

unsigned int n_arestas(grafo *g);

//------------------------------------------------------------------------------
// devolve o número de componentes em g

unsigned int n_componentes(grafo *g);

//------------------------------------------------------------------------------
// devolve uma "string" com os diâmetros dos componentes de g separados por brancos
// em ordem não decrescente

char *diametros(grafo *g);

//------------------------------------------------------------------------------
// devolve uma "string" com os nomes dos vértices de corte de g em
// ordem alfabética, separados por brancos

char *vertices_corte(grafo *g);

//------------------------------------------------------------------------------
// devolve uma "string" com as arestas de corte de g em ordem alfabética, separadas por brancos
// cada aresta é o par de nomes de seus vértices em ordem alfabética, separadas por brancos
//
// por exemplo, se as arestas de corte são {z, a}, {x, b} e {y, c}, a resposta será a string
// "a z b x c y"

char *arestas_corte(grafo *g);

#endif
