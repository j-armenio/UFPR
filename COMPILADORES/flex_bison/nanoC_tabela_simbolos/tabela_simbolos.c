#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"
#include "compilador.h"

// Essas funções de novo_simbolo, criam diferentes versões de símbolos, com dados parciais ou completos:

// Cria um símbolo sem tipo, sem escopo, sem categoria
// Útil para testes ou etapas parciais
struct simbolo * novo_simbolo1(char *lexema){
    struct simbolo * novo = malloc(sizeof(struct simbolo));
    novo->lexema = lexema;
    return novo;
}

// Esse especific se é uma VARIAVEL ou FUNCAO
struct simbolo * novo_simbolo2(char *lexema, TipoSimbolo tipo_simb){
    struct simbolo * novo = malloc(sizeof(struct simbolo));
    novo->lexema = lexema;
    novo->tipo_simb = tipo_simb;
    return novo;
}

// Cria um símbolo com nome, tipo_simb e escopo
// Usado em variáveis sem tipo ainda (tipo será atualizado depois)
struct simbolo * novo_simbolo3(char *lexema, TipoSimbolo tipo_simb, int escopo){
    struct simbolo * novo = malloc(sizeof(struct simbolo));
    novo->lexema = lexema;
    novo->tipo_simb = tipo_simb;
    novo->escopo = escopo;
    return novo;
}

// Cria símbolos com todas as informações
struct simbolo * novo_simbolo4(char *lexema, TipoSimbolo tipo_simb, int escopo, Tipo tipo){
    struct simbolo * novo = malloc(sizeof(struct simbolo));
    novo->lexema = lexema;
    novo->tipo_simb = tipo_simb;
    novo->escopo = escopo;
    novo->tipo = tipo;
    return novo;
}

// Insere um novo símbolo no fim de uma lista
// Essa lista é usada para agrupar variáveis em declarações do tipo: int x, y, z;
struct lista_simbolo * insere_lista_simbolo(struct lista_simbolo * lista, struct simbolo * simb){
    // insere no final
    struct lista_simbolo *aux, *novo = malloc(sizeof(struct lista_simbolo));
    novo->simb = simb;
    novo->proximo = NULL;
    if(lista == NULL){
        return novo;
    }
    aux = lista;
    while(aux->proximo != NULL)
        aux = aux->proximo;
    aux->proximo = novo;
    return lista;
}

// Aplica o mesmo tipo para todos simbolos de uma lista
void atualiza_tipo_simbolos(struct lista_simbolo * lista, Tipo t){
    while(lista != NULL){
        lista->simb->tipo = t;
        lista = lista->proximo;
    }
}

// Libera memória da lista encadeada
void free_lista_simbolo(struct lista_simbolo * lista){
    if(lista == NULL)
        return;
    free_lista_simbolo(lista->proximo);
    free(lista);
}

// Verifica se o simbolo já foi declarado no mesmo escopo,
//   se sim, chama yyerror()
//   caso contrário, insere o novo símbolo no topo da tabela (pilha)
struct tabela_simbolos * insere_simbolo_ts(struct tabela_simbolos * ts, struct simbolo * simb){
    struct simbolo *simb_busca = busca_simbolo(ts, simb->lexema);
    if(simb_busca != NULL && simb_busca->escopo == simb->escopo){
        char erro[500];
        sprintf(erro, "simbolo '%s' ja declarado antes", simb->lexema);
        yyerror(erro);
    }
    struct tabela_simbolos * novo = malloc(sizeof(struct tabela_simbolos));
    novo->simb = simb;
    novo->proximo = ts;
    return novo;
}

// Insere todos os símbolos da lista na tabela de símbolos, um por um, e dps libera a memória da lista
struct tabela_simbolos * insere_simbolos_ts(struct tabela_simbolos * ts, struct lista_simbolo * lista){
    struct lista_simbolo * aux = lista;
    while(aux != NULL){
        ts = insere_simbolo_ts(ts, aux->simb);
        aux = aux->proximo;
    }
    free_lista_simbolo(lista);
    return ts;
}

// Procura por um nome(lexema) na tabela e, se encontrar, retorna o símbolo correspondente
struct simbolo * busca_simbolo(struct tabela_simbolos * ts, char *lexema){
    while(ts != NULL){
        if(strcmp(ts->simb->lexema, lexema) == 0)
            return ts->simb;
        ts = ts->proximo;
    }
    return NULL;
}

// Remove todos símbolos do escopo fornecido
// Usado quando sai de uma função para limpar variáveis locais
struct tabela_simbolos * remove_simbolos(struct tabela_simbolos * ts, int escopo){
    struct tabela_simbolos * aux;
    while(ts != NULL && ts->simb->escopo == escopo){
        free(ts->simb);
        aux = ts;
        ts = ts->proximo;
        free(aux);
    }
    return ts;
}

// Constroi uma lista de tipos de argumentos e associa ao simbolo da função
// Permite verificar tipos de parametros depois
void insere_func_args(struct simbolo * funcao, struct lista_simbolo * args){
    struct lista_args * novo, * ultimo;
    while(args != NULL){
        novo = malloc(sizeof(struct lista_args));
        novo->tipo = args->simb->tipo;
        novo->proximo = NULL;
        if(funcao->args == NULL)
            funcao->args = novo;
        else
            ultimo->proximo = novo;
        ultimo = novo;
        args = args->proximo;
    }
}

// Imprime o nome textual de um tipo
void imprime_tipo(FILE *fp, Tipo tipo){
    switch (tipo) {
        case INTEIRO: fprintf(fp, "int"); break;
        case REAL: fprintf(fp, "float"); break;
        case VAZIO: fprintf(fp, "void"); break;
    }
}

// Imprime uma função com seus argumentos e tipo de retorno
void imprime_funcao(FILE *fp, struct simbolo *func){
    fprintf(fp,"FUNCAO; lexema=%s; escopo=%d; tipo=",func->lexema,func->escopo);
    imprime_tipo(fp, func->tipo);
    fprintf(fp, "; args:{");
    struct lista_args *args = func->args;
    while(args != NULL){
        imprime_tipo(fp, args->tipo);
        if(args->proximo != NULL){
            fprintf(fp, ", ");
        }
        args = args->proximo;
    }
    fprintf(fp, "}\n");
}

// Imprime informações de uma variável
void imprime_variavel(FILE *fp, struct simbolo *var){
    fprintf(fp,"VARIAVEL; lexema=%s; escopo=%d; tipo=",var->lexema,var->escopo);
    imprime_tipo(fp, var->tipo);
    fprintf(fp, "\n");
}

// Percorre a tabela e imprime todos os símbolos, da pilha do escopo atual para baixo
void imprime_tabela_simbolos(FILE *fp, struct tabela_simbolos * ts){
    fprintf(fp, "--------------TS--------------\n");
    while(ts != NULL){
        if(ts->simb->tipo_simb == FUNCAO)
            imprime_funcao(fp, ts->simb);
        else
            imprime_variavel(fp, ts->simb);
        ts = ts->proximo;
        fprintf(fp, "---------------\n");
    }
    fprintf(fp, "--------------FIM-TS----------\n");
}
