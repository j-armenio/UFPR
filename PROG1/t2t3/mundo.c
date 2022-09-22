#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "liblef.h"
#include "libaleat.h"
#include "libentidades.h"

#define N_TAMANHO_MUNDO 20000
#define N_RUMORES_MUNDO 30
#define N_PESSOAS 100
#define N_LOCAIS 8
#define FIM_DO_MUNDO 34944
#define CHEGADA 1
#define SAIDA 2
#define RUMOR 3
#define FIM 4

/* Retorna o maior elemento */
int MAX(int a, int b) { return a > b ? a : b; }

/* Definicao dos dados dos eventos */
typedef struct ev_chegada {
    int id_pessoa;
    int id_local;
} ev_chegada_t;

typedef struct ev_rumor {
    int id_pessoa;
    int id_local;
    conjunto_t *crc_pes;
} ev_rumor_t;

/* Desalocar memoria (esqueça) */
void destroi_chegada(void *ev_chegada){}
void destroi_rumor(void *ev_rumor){}

int inicializa_entidades(mundo_t *mundo, conjunto_t *cj_rumores, pessoa_t *pessoas, local_t *locais)
{
    /* Inicializando cj_rumores */
    int i;
    for (i = 0; i < N_RUMORES_MUNDO; i++)
        insere_conjunto(cj_rumores, i);

    /* Inicializando populacao */

    for (i = 0; i < N_PESSOAS; i++){
        pessoas[i].id = i;
        pessoas[i].extroversao = ALEAT(0, 100);
        pessoas[i].paciencia = ALEAT(0, 100);
        pessoas[i].idade = ALEAT(18, 100);
        pessoas[i].crc = cria_subconjunto(cj_rumores, ALEAT(1, 5));
    }

    /* Inicializando locais */
    for (i = 0; i < N_LOCAIS; i++){
        locais[i].id = i;
        locais[i].lotacao_max = ALEAT(5, 30);
        locais[i].publico = cria_conjunto(locais[i].lotacao_max);
        fila_t *fila = cria_fila();
        locais[i].fila = fila;
        locais[i].coord_x = ALEAT(0, N_TAMANHO_MUNDO-1);
        locais[i].coord_y = ALEAT(0, N_TAMANHO_MUNDO-1);
    }

    /* Inicializando mundo */
    mundo->tempo_atual = 0;
    mundo->npessoas = N_PESSOAS;
    mundo->nlocais = N_LOCAIS;
    mundo->rumores = cj_rumores;
    mundo->pessoas = pessoas;
    mundo->locais = locais;
    mundo->coord_x = N_TAMANHO_MUNDO;
    mundo->coord_y = N_TAMANHO_MUNDO;

    return 1;
}

/* Alocando espaco para os eventos */
int aloca_dados_chegada(evento_t *evento)
{
    if (! (evento->dados = (ev_chegada_t*) malloc (sizeof(ev_chegada_t))))
        return 0;
    return 1;
}

int aloca_dados_rumor(evento_t *evento)
{
    if (! (evento->dados = (ev_rumor_t*) malloc (sizeof(ev_rumor_t))))
        return 0;
    return 1;

    ((ev_rumor_t*) evento->dados)->crc_pes = cria_conjunto(30);
}

/* Inserindo dados nos eventos */
int insere_dados_chegada(evento_t *evento, int tempo, int id_pessoa, int id_local)
{
    evento->tempo = tempo;
    evento->tipo = CHEGADA;
    evento->tamanho = sizeof(ev_chegada_t);
    evento->destroidados = destroi_chegada;
    ((ev_chegada_t*) evento->dados)->id_pessoa = id_pessoa;
    ((ev_chegada_t*) evento->dados)->id_local = id_local;
    return 1;
}

int insere_dados_saida(evento_t *evento, int tempo, int id_pessoa, int id_local)
{
    evento->tempo = tempo;
    evento->tipo = SAIDA;
    evento->tamanho = sizeof(ev_chegada_t);
    evento->destroidados = destroi_chegada;
    ((ev_chegada_t*) evento->dados)->id_pessoa = id_pessoa;
    ((ev_chegada_t*) evento->dados)->id_local = id_local;
    return 1;
}

int insere_dados_rumor(evento_t *evento, int tempo, int id_pessoa, int id_local, conjunto_t *subcj)
{
    evento->tempo = tempo;
    evento->tipo = RUMOR;
    evento->tamanho = sizeof(ev_rumor_t);
    evento->destroidados = destroi_rumor;
    ((ev_rumor_t*) evento->dados)->id_pessoa = id_pessoa;
    ((ev_rumor_t*) evento->dados)->id_local = id_local;
    ((ev_rumor_t*) evento->dados)->crc_pes = subcj;
    return 1;
}

int insere_dados_fimDoMundo(evento_t *evento, int tempo)
{
    evento->tempo = FIM_DO_MUNDO;
    evento->tipo = FIM;
    return 1;
}

/* Tratando os eventos */
int trata_ev_chegada(evento_t *evento, mundo_t *mundo, lef_t *lef)
{
    int id_local = ((ev_chegada_t*) evento->dados)->id_local;
    int id_pessoa = ((ev_chegada_t*) evento->dados)->id_pessoa;
    local_t local = mundo->locais[id_local];
    pessoa_t pessoa = mundo->pessoas[id_pessoa];
    evento_t ev_dados_saida;
    evento_t ev_dados_rumor;
    conjunto_t *subcj;

    aloca_dados_rumor(&ev_dados_rumor);
    aloca_dados_chegada(&ev_dados_saida);

    if (cardinalidade(local.publico) == local.lotacao_max){ /* LOCAL LOTADO */
        if (pessoa.paciencia/4 - tamanho_fila(local.fila) > 0){
            insere_fila(local.fila, id_pessoa);
            
            printf("%5d:CHEGA Pessoa %2d Local %1d (%2d/%2d), FILA %d\n", mundo->tempo_atual, id_pessoa, id_local, cardinalidade(local.publico), local.lotacao_max, tamanho_fila(local.fila));
            return 1;
        }
        else{
            printf("%5d:CHEGA Pessoa %2d Local %1d (%2d/%2d), DESISTE\n", mundo->tempo_atual, id_pessoa, id_local, cardinalidade(local.publico), local.lotacao_max);

            /* Cria evento de saida */
            insere_dados_saida(&ev_dados_saida, mundo->tempo_atual+0, id_pessoa, id_local);
            adiciona_ordem_lef(lef, &ev_dados_saida);
            return 1;
        }
    } else { /* LOCAL NAO LOTADO */
        printf("%5d:CHEGA Pessoa %2d Local %1d (%2d/%2d), ENTRA\n", mundo->tempo_atual, id_pessoa, id_local, cardinalidade(local.publico), local.lotacao_max);
        insere_conjunto(local.publico, id_pessoa);
        int tpl = MAX(1, pessoa.paciencia/10+ALEAT(-2, 6));
        insere_dados_saida(&ev_dados_saida, mundo->tempo_atual+tpl, id_pessoa, id_local);
        adiciona_ordem_lef(lef, &ev_dados_saida);

        /* cria evento de disseminacao */
        int nrd = pessoa.extroversao/10;
        subcj = cria_subconjunto(pessoa.crc, nrd);
        aloca_dados_rumor(&ev_dados_rumor);
        insere_dados_rumor(&ev_dados_rumor, mundo->tempo_atual+ALEAT(0,tpl), id_pessoa, id_local, subcj);
        adiciona_ordem_lef(lef, &ev_dados_rumor);
        return 1;
    }
    return 0;
}

int trata_ev_saida(evento_t *evento, mundo_t *mundo, lef_t *lef)
{
    int id_local = ((ev_chegada_t*) evento->dados)->id_local;
    int id_pessoa = ((ev_chegada_t*) evento->dados)->id_pessoa;
    local_t local = mundo->locais[id_local];
    pessoa_t pessoa = mundo->pessoas[id_pessoa];
    evento_t ev_dados_chegada;

    aloca_dados_chegada(&ev_dados_chegada);

    printf("%5d:SAIDA Pessoa %2d Local %1d (%2d/%2d)\n", mundo->tempo_atual, id_pessoa, id_local, cardinalidade(local.publico), local.lotacao_max);

    /* Como a pessoa esta saindo precisa tirar ela do local anterior */
    retira_conjunto(local.publico, id_pessoa);

    /* cria evento de chegada para a primeira pessoa na fila em id_local */    
    /* if (!fila_vazia(local.fila)){
        int pessoa_fila;
        retira_fila(local.fila, &pessoa_fila);
        insere_dados_chegada(&ev_dados_chegada, mundo->tempo_atual+0, pessoa_fila, id_local);
        adiciona_inicio_lef(lef, &ev_dados_chegada);
    } */


    /* Criar evento de chegada para id_pessoa em id_destino */
    int id_destino = ALEAT(0, N_LOCAIS);
    local_t destino = mundo->locais[id_destino];

    int distancia = sqrt(pow(destino.coord_x - local.coord_x, 2) + pow(destino.coord_y - local.coord_y, 2));

    int velocidade = 100 - MAX(0, pessoa.idade-40);
    int tdl = distancia / velocidade;

    insere_dados_chegada(&ev_dados_chegada, mundo->tempo_atual+tdl/15, id_pessoa, destino.id);
    adiciona_ordem_lef(lef, &ev_dados_chegada);
    return 1;
}

int trata_ev_rumor(evento_t *evento, mundo_t *mundo, lef_t *lef)
{
    int id_pessoa_escuta;
    int it_rumor;
    int it_pessoa;
    int id_rumor;
    int id_local = ((ev_chegada_t*) evento->dados)->id_local;
    int id_pessoa = ((ev_chegada_t*) evento->dados)->id_pessoa;
    local_t local = mundo->locais[id_local];
    conjunto_t *cj_rumor = ((ev_rumor_t*) evento->dados)->crc_pes;

    printf("%5d:RUMOR Pessoa %2d Local %1d ", mundo->tempo_atual, id_pessoa, id_local);

    /* Rodar todas pessoas do publico com cada rumor e verificar se elas vao escutar ou nao */
    iniciar_iterador(local.publico);

    for (it_pessoa = 0; it_pessoa < cardinalidade(local.publico); it_pessoa++){
        incrementar_iterador(local.publico, &id_pessoa_escuta);
        iniciar_iterador(cj_rumor);
        /* Rodar todos rumores do evento_rumor */
        for (it_rumor = 0; it_rumor < cardinalidade(cj_rumor); it_rumor++){
            incrementar_iterador(cj_rumor, &id_rumor);
            if (ALEAT(0,100) < mundo->pessoas[id_pessoa_escuta].extroversao){
                if (insere_conjunto(mundo->pessoas[id_pessoa_escuta].crc, id_rumor)){
                    printf("(P%d/R%d) ", mundo->pessoas[id_pessoa_escuta].id, id_rumor);
                }
            }
        }
    }
    printf("\n");
    return 1;
}

int main(){
    srand(0);

    lef_t *lef = cria_lef();
    mundo_t *mundo;
    pessoa_t *pessoas;
    local_t *locais;
    conjunto_t *cj_rumores = cria_conjunto(N_RUMORES_MUNDO);

    /* Aloca espaco para as entidades. */
    if (! (mundo = malloc(sizeof(mundo_t)))){
        printf("Falha em alocar mundo.");
        return 0;
    }
    if (! (pessoas = malloc(sizeof(pessoa_t) * N_PESSOAS))){
        printf("Falha em alocar vetor pessoas.");
        return 0;
    }
    if (! (locais = malloc(sizeof(local_t) * N_LOCAIS))){
        printf("Falha em alocar vetor locais.");
        return 0;
    }

    /* Inicializa e preenche todas entidades e as coloca no mundo */
    inicializa_entidades(mundo, cj_rumores, pessoas, locais);

    /* Declara os eventos necessarios para comecar a preencher a lef */
    evento_t *ev_atual;
    evento_t ev_dados_chegada;
    aloca_dados_chegada(&ev_dados_chegada);

    /* Cria um evento de chegada para cada pessoa do mundo e insere na lef */
    int i;
    for (i = 0; i < N_PESSOAS; i++){
        insere_dados_chegada(&ev_dados_chegada, ALEAT(0, 96*7), i, ALEAT(0, N_LOCAIS-1));
        adiciona_ordem_lef(lef, &ev_dados_chegada);
    }

    /* Cria e insere fim do mundo na LEF */
    insere_dados_fimDoMundo(&ev_dados_chegada, FIM_DO_MUNDO);
    adiciona_ordem_lef(lef, &ev_dados_chegada);

    while ((ev_atual = obtem_primeiro_lef(lef))){
        mundo->tempo_atual = ev_atual->tempo;

        switch (ev_atual->tipo)
        {
        case CHEGADA:
            trata_ev_chegada(ev_atual, mundo, lef);
            break;
        case SAIDA:
            trata_ev_saida(ev_atual, mundo, lef);
            break;
        case RUMOR:
            trata_ev_rumor(ev_atual, mundo, lef);
            break;
        case FIM:
            printf("!!!!!!!APOCALIPSE CHEGOU!!!!!!!\n");
            return 1;
            break;
        }
    }
    return 0;
}
