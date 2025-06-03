#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

typedef enum Tipo_e {INTEIRO, REAL, VAZIO} Tipo;
typedef enum TipoSimbolo_e {VARIAVEL, FUNCAO} TipoSimbolo;

struct simbolo {
    char * lexema;
    Tipo tipo;
    TipoSimbolo tipo_simb;
    int escopo;
    struct lista_args * args;
};

struct lista_args {
    Tipo tipo;
    struct lista_args * proximo;
};

struct lista_simbolo{
    struct simbolo * simb;
    struct lista_simbolo * proximo;
};

struct tabela_simbolos {
    struct simbolo * simb;
    struct tabela_simbolos * proximo;
};

struct infoLLVM {
    char *operando;      // "%4", "35", "1.74"
    Tipo tipo_pascal;    // i32 ou double
    char *tipo_llvm_str; // INTEIRO ou REAL
    char *var_name;      // se veio de ID, contem lexema gerador; senao NULL
};

struct lista_info {
    struct infoLLVM info;
    struct lista_info *proximo;
};

// sem overload em ANSI C!
struct simbolo * novo_simbolo1(char *lexema);
struct simbolo * novo_simbolo2(char *lexema, TipoSimbolo tipo_simb);
struct simbolo * novo_simbolo3(char *lexema, TipoSimbolo tipo_simb, int escopo);
struct simbolo * novo_simbolo4(char *lexema, TipoSimbolo tipo_simb, int escopo, Tipo tipo);
struct simbolo *novo_simbolo_temp(char *lexema, Tipo tipo);

struct lista_simbolo * insere_lista_simbolo(struct lista_simbolo * lista, struct simbolo * simb);
struct lista_simbolo *novo_simbolo_arg(char *lexema, Tipo tipo_pascal);
void atualiza_tipo_simbolos(struct lista_simbolo * lista, Tipo t);
void atualiza_escopo_simbolos(struct lista_simbolo *lista, int novo_escopo);
void insere_func_args(struct simbolo * funcao, struct lista_simbolo * args);
void free_lista_simbolo(struct lista_simbolo * lista);
struct lista_simbolo* concatena_listas_simbolos(struct lista_simbolo *l1, struct lista_simbolo *l2);

struct tabela_simbolos * insere_simbolo_ts(struct tabela_simbolos * ts, struct simbolo * simb);
struct tabela_simbolos * insere_simbolos_ts(struct tabela_simbolos * ts, struct lista_simbolo * lista);
struct simbolo * busca_simbolo(struct tabela_simbolos * ts, char *lexema);
struct simbolo *busca_funcao(struct tabela_simbolos * ts, char *lexema);
struct tabela_simbolos * remove_simbolos(struct tabela_simbolos * ts, int escopo);
struct tabela_simbolos *adiciona_procedures_nativas(struct tabela_simbolos *tab_simbolos);

void imprime_tabela_simbolos(FILE * fp, struct tabela_simbolos * ts);

void imprime_lista_simbolos(struct lista_simbolo *l);
void imprime_funcao_debug(struct simbolo *func);

#endif
