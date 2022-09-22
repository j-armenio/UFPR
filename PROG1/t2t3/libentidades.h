#include <stdio.h>
#include "libconjunto.h"
#include "libfila.h"

typedef struct pessoa {
    int id;                  /* int que identifica cada pessoa */
    int extroversao;         /* [0..100] quanto maior + propenso a disseminar rumores */
    int paciencia;           /* [0..100] permanencia em locais e filas */
    int idade;               /* [18..100] tempo de deslocamento entre lugares */
    conjunto_t *crc;         /* conjunto de rumores conhecidos por uma pessoa */
} pessoa_t;

typedef struct local {
    int id;                  /* int que identifica o local */
    int lotacao_max;         /* num max de pessoas no lugar */
    conjunto_t *publico;     /* conj de pessoas(id) atualmente em um lugar */
    fila_t *fila;            /* fila para esperar caso local esteja lotado */
    int coord_x;             /* coordenada do eixo horizontal x */
    int coord_y;             /* coordenada do eixo vertical x */
} local_t;

typedef struct mundo {
    int tempo_atual;         /* int positivo indicando tempo real da simulacao (1 = 15 minutos)*/
    pessoa_t *pessoas;       /* vetor com todas as pessoas(id) */
    local_t *locais;         /* vetor todos os locais(id) */
    conjunto_t *rumores;     /* conj com todos rumores */
    int npessoas;            /* num total de pessoas */
    int nlocais;             /* num total de locais */
    int coord_x;             /* eixo x da coord max do plano cartesiano dos locais */ 
    int coord_y;             /* eixo y da coord max do plano cartesiano dos locais */
} mundo_t;