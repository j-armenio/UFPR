#ifndef LLVMLIB_H
#define LLVMLIB_H

#include<stdio.h>
#include "tabela_simbolos.h"

struct lista_info *ll_insere_info_lista(
    struct lista_info *l, 
    struct infoLLVM info);
void ll_imprime_infoLLVM(struct infoLLVM info);
const char *ll_tipo_pascal_para_llvm(Tipo t);
void ll_insere_funcoes_nativas(FILE *out_file);
char *ll_novo_temporario();
void ll_resetar_contador_temporario();
char *ll_novo_rotulo();
void ll_escreve_chamada_proc(
    FILE *out_file, 
    struct tabela_simbolos *tab_simbolos, 
    const char *nome, 
    struct lista_info *args,
    struct simbolo* proc);
struct infoLLVM ll_operacao_binaria(
    FILE *out_file,
    struct tabela_simbolos *tab_simbolos,
    struct infoLLVM op1_info,
    struct infoLLVM op2_info,
    const char *op_pascal);

#endif
