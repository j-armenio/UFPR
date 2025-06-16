#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

typedef enum Tipo_e {INTEIRO, REAL, VAZIO} Tipo;
typedef enum TipoSimbolo_e {VARIAVEL, FUNCAO} TipoSimbolo;

// Registro de cada símbolo
struct simbolo {
    char * lexema;            // Nome do identificador: "x", "main", etc
    Tipo tipo;                // INTEIRO, REAL, ou VAZIO (p função void)
    TipoSimbolo tipo_simb;    // VARIAVEL ou FUNCAO
    int escopo;               // Nível de escopo (0=global, 1=dentro de função, etc)
    struct lista_args * args; // Lista de tipos dos argumentos, se for função
};

struct lista_args { // Se for função, lista do tipo dos argumentos
    Tipo tipo;
    struct lista_args * proximo;
};

// Antes de inserir símbolos na tabela global, eles são empilhados em uma lista
struct lista_simbolo{
    struct simbolo * simb;
    struct lista_simbolo * proximo;
};

// Funciona, nesse caso, como uma pilha ligada de símbolos
// A estrutura é uma lista encadeada
// Cada elemento é um símbolo com todos os dados associados
// Quando entra em um novo escopo (ex: função), o escopo é aumentado e, ao final, os
//  símbolos com aquele escopo são removidos
struct tabela_simbolos {
    struct simbolo * simb;
    struct tabela_simbolos * proximo;
};

// sem overload em ANSI C!
struct simbolo * novo_simbolo1(char *lexema);
struct simbolo * novo_simbolo2(char *lexema, TipoSimbolo tipo_simb);
struct simbolo * novo_simbolo3(char *lexema, TipoSimbolo tipo_simb, int escopo);
struct simbolo * novo_simbolo4(char *lexema, TipoSimbolo tipo_simb, int escopo, Tipo tipo);

struct lista_simbolo * insere_lista_simbolo(struct lista_simbolo * lista, struct simbolo * simb);
void atualiza_tipo_simbolos(struct lista_simbolo * lista, Tipo t);
void insere_func_args(struct simbolo * funcao, struct lista_simbolo * args);
void free_lista_simbolo(struct lista_simbolo * lista);

struct tabela_simbolos * insere_simbolo_ts(struct tabela_simbolos * ts, struct simbolo * simb);
struct tabela_simbolos * insere_simbolos_ts(struct tabela_simbolos * ts, struct lista_simbolo * lista);
struct simbolo * busca_simbolo(struct tabela_simbolos * ts, char *lexema);
struct tabela_simbolos * remove_simbolos(struct tabela_simbolos * ts, int escopo);

void imprime_tabela_simbolos(FILE * fp, struct tabela_simbolos * ts);
#endif

