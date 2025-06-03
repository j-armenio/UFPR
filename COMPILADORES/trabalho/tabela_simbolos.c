#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"
#include "compilador.h"

/* ======== MINHAS ======== */

struct tabela_simbolos *adiciona_procedures_nativas(struct tabela_simbolos *tab_simbolos) {
    // Adiciona as procedures nativas à tabela de símbolos
    struct simbolo *read_proc = novo_simbolo3("read", FUNCAO, 0);
    read_proc->tipo = VAZIO;
    tab_simbolos = insere_simbolo_ts(tab_simbolos, read_proc);

    struct simbolo *write_proc = novo_simbolo3("write", FUNCAO, 0);
    write_proc->tipo = VAZIO;
    tab_simbolos = insere_simbolo_ts(tab_simbolos, write_proc);

    struct simbolo *writeln_proc = novo_simbolo3("writeln", FUNCAO, 0);
    writeln_proc->tipo = VAZIO;
    tab_simbolos = insere_simbolo_ts(tab_simbolos, writeln_proc);

    struct simbolo *readln_proc = novo_simbolo3("readln", FUNCAO, 0);
    readln_proc->tipo = VAZIO;
    tab_simbolos = insere_simbolo_ts(tab_simbolos, readln_proc);

    return tab_simbolos;
}

void imprime_tipo_debug(Tipo tipo){
    switch (tipo) {
        case INTEIRO: printf("int"); break;
        case REAL: printf("float"); break;
        case VAZIO: printf("void"); break;
    }
}

void imprime_tipo_simb(TipoSimbolo tipo_simb){
    switch (tipo_simb) {
        case VARIAVEL: printf("variavel"); break;
        case FUNCAO: printf("função"); break;
    }
}

void imprime_funcao_debug(struct simbolo *func){
    printf("FUNCAO; lexema=%s; escopo=%d; tipo=",func->lexema,func->escopo);
    imprime_tipo_debug(func->tipo);
    printf("; args:{");
    struct lista_args *args = func->args;
    while(args != NULL){
        imprime_tipo_debug(args->tipo);
        if(args->proximo != NULL){
            printf(", ");
        }
        args = args->proximo;
    }
    printf("}\n");
}

void imprime_lista_simbolos(struct lista_simbolo *l){
    printf("Lista de simbolos:\n");
    while (l != NULL) {
        printf("  Simbolo: %s; tipo_simb=", l->simb->lexema);
        imprime_tipo_simb(l->simb->tipo_simb);
        printf("; escopo=%d; tipo=", l->simb->escopo);
        imprime_tipo_debug(l->simb->tipo);
        printf("\n");
        l = l->proximo;
    }
    printf("Fim da lista de simbolos.\n");
}

struct lista_simbolo* concatena_listas_simbolos(struct lista_simbolo *l1, struct lista_simbolo *l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    struct lista_simbolo *aux = l1;
    while (aux->proximo)
        aux = aux->proximo;
    aux->proximo = l2;

    return l1;
}

/* ================== */

struct simbolo * novo_simbolo1(char *lexema){
    //printf("Criando1 simbolo %s\n", lexema);
    struct simbolo * novo = malloc(sizeof(struct simbolo));
    novo->lexema = lexema;
    return novo;
}

struct simbolo * novo_simbolo2(char *lexema, TipoSimbolo tipo_simb){
    //printf("Criando2 simbolo %s; tipoSimb=", lexema);
    //imprime_tipo_simb(tipo_simb);
    //printf("\n");

    struct simbolo * novo = malloc(sizeof(struct simbolo));
    novo->lexema = lexema;
    novo->tipo_simb = tipo_simb;
    return novo;
}

struct simbolo * novo_simbolo3(char *lexema, TipoSimbolo tipo_simb, int escopo){
    //printf("Criando3 simbolo %s; tipoSimb=", lexema);
    //imprime_tipo_simb(tipo_simb);
    //printf("\n");
    struct simbolo * novo = malloc(sizeof(struct simbolo));
    novo->lexema = lexema;
    novo->tipo_simb = tipo_simb;
    novo->escopo = escopo;
    return novo;
}
struct simbolo * novo_simbolo4(char *lexema, TipoSimbolo tipo_simb, int escopo, Tipo tipo){
    //printf("Criando4 simbolo %s; tipoSimb=", lexema);
    //imprime_tipo_simb(tipo_simb);
    //printf("\n");
    struct simbolo * novo = malloc(sizeof(struct simbolo));
    novo->lexema = lexema;
    novo->tipo_simb = tipo_simb;
    novo->escopo = escopo;
    novo->tipo = tipo;
    return novo;
}
struct simbolo *novo_simbolo_temp(char *lexema, Tipo tipo) {
    struct simbolo *s = malloc(sizeof(struct simbolo));
    s->lexema = lexema;
    s->tipo_simb = -1; // indefinido ou ignorado
    s->escopo = -1;
    return s;
}

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

struct lista_simbolo *novo_simbolo_arg(char *lexema, Tipo tipo_pascal)
{
    struct lista_simbolo *no = malloc(sizeof(*no));
    no->simb = malloc(sizeof(*no->simb));
    no->simb->lexema = strdup(lexema);
    no->simb->tipo = tipo_pascal;
    no->simb->tipo_simb = VARIAVEL;
    no->simb->escopo = 0; // simplificado
    no->proximo = NULL;
    return no;
}

void atualiza_tipo_simbolos(struct lista_simbolo * lista, Tipo t){
    while(lista != NULL){
        lista->simb->tipo = t;
        lista = lista->proximo;
    }
}

void atualiza_escopo_simbolos(struct lista_simbolo *lista, int novo_escopo){
    while (lista){
        lista->simb->escopo = novo_escopo;
        lista = lista->proximo;
    }
}


void free_lista_simbolo(struct lista_simbolo * lista){
    if(lista == NULL)
        return;
    free_lista_simbolo(lista->proximo);
    free(lista);
}

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
struct tabela_simbolos * insere_simbolos_ts(struct tabela_simbolos * ts, struct lista_simbolo * lista){
    struct lista_simbolo * aux = lista;
    while(aux != NULL){
        ts = insere_simbolo_ts(ts, aux->simb);
        aux = aux->proximo;
    }
    free_lista_simbolo(lista);
    return ts;
}

struct simbolo * busca_simbolo(struct tabela_simbolos * ts, char *lexema){
    while(ts != NULL){
        if(strcmp(ts->simb->lexema, lexema) == 0)
            return ts->simb;
        ts = ts->proximo;
    }
    return NULL;
}

struct simbolo *busca_funcao(struct tabela_simbolos * ts, char *lexema){
    while(ts != NULL){
        if(strcmp(ts->simb->lexema, lexema) == 0 && ts->simb->tipo_simb == FUNCAO)
            return ts->simb;
        ts = ts->proximo;
    }
    return NULL;
}

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

void imprime_tipo(FILE *fp, Tipo tipo){
    switch (tipo) {
        case INTEIRO: fprintf(fp, "int"); break;
        case REAL: fprintf(fp, "float"); break;
        case VAZIO: fprintf(fp, "void"); break;
    }
}
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

void imprime_variavel(FILE *fp, struct simbolo *var){
    fprintf(fp,"VARIAVEL; lexema=%s; escopo=%d; tipo=",var->lexema,var->escopo);
    imprime_tipo(fp, var->tipo);
    fprintf(fp, "\n");
}
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

