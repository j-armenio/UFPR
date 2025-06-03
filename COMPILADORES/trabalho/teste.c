#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvmlib.h"
#include "tabela_simbolos.h"

int contador_temp = 0;

ListaInfo *push_info(ListaInfo *l, struct InfoLLVM info) {
    ListaInfo *novo = malloc(sizeof *novo);
    if (!novo) {
        fprintf(stderr, "Erro de memória em push_info\n");
        exit(1);
    }
    novo->info = info;
    novo->prox = l;
    return novo;
}

void ll_escreve_chamada_proc(FILE *out_file,
                             struct tabela_simbolos *tab_simbolos,
                             const char *nome,
                             ListaInfo *args_info)
{
    if (strcmp(nome, "read") == 0 || strcmp(nome, "readln") == 0) {
        for (ListaInfo *it = args_info; it; it = it->prox) {
            struct InfoLLVM info = it->info;
            if (!info.var_name) {
                yyerror("read/readln exige IDENTIFICADOR puro");
            }
            struct simbolo *s = busca_simbolo(tab_simbolos, info.var_name);
            if (!s) yyerror("variável não declarada em read()");
            char ptr_dest[64];
            if (s->escopo == 0)
                sprintf(ptr_dest, "@%s", s->lexema);
            else
                sprintf(ptr_dest, "%%%s", s->lexema);

            const char *fmt = (s->tipo == REAL ? "@read_float" : "@read_int");
            fprintf(out_file, "    call i32 @scanf(ptr %s, ptr %s)\n",
                    fmt, ptr_dest);
            printf(    "    call i32 @scanf(ptr %s, ptr %s)\n",
                       fmt, ptr_dest);
        }
    }
    else if (strcmp(nome, "write") == 0 || strcmp(nome, "writeln") == 0) {
        for (ListaInfo *it = args_info; it; it = it->prox) {
            struct InfoLLVM info = it->info;
            if (info.var_name) {
                struct simbolo *s = busca_simbolo(tab_simbolos, info.var_name);
                if (!s) yyerror("variável não declarada em write()");
                char ptr_src[64];
                if (s->escopo == 0)
                    sprintf(ptr_src, "@%s", s->lexema);
                else
                    sprintf(ptr_src, "%%%s", s->lexema);
                const char *fmt = (s->tipo == REAL ? "@write_float" : "@write_int");
                fprintf(out_file, "    call i32 @printf(ptr %s, %s %s)\n",
                        fmt,
                        ll_tipo_pascal_para_llvm(s->tipo),
                        ptr_src);
                printf(    "    call i32 @printf(ptr %s, %s %s)\n",
                           fmt,
                           ll_tipo_pascal_para_llvm(s->tipo),
                           ptr_src);
            } else {
                const char *fmt = (info.tipo_pascal == REAL ? "@write_float" : "@write_int");
                fprintf(out_file, "    call i32 @printf(ptr %s, %s %s)\n",
                        fmt,
                        info.tipo_llvm_str,
                        info.operando);
                printf(    "    call i32 @printf(ptr %s, %s %s)\n",
                           fmt,
                           info.tipo_llvm_str,
                           info.operando);
            }
        }
    }
    else {
        // chamada de função de usuário (retorno != VAZIO)
        char buffer_args[512] = "";
        int primeiro = 1;
        for (ListaInfo *it = args_info; it; it = it->prox) {
            if (!primeiro) strcat(buffer_args, ", ");
            primeiro = 0;
            struct InfoLLVM info = it->info;
            if (info.var_name) {
                struct simbolo *s = busca_simbolo(tab_simbolos, info.var_name);
                if (!s) yyerror("variável não declarada em chamada de função");
                const char *tipo_str = ll_tipo_pascal_para_llvm(s->tipo);
                char arg_txt[128];
                if (s->escopo == 0)
                    sprintf(arg_txt, "%s @%s", tipo_str, s->lexema);
                else
                    sprintf(arg_txt, "%s %%%s", tipo_str, s->lexema);
                strcat(buffer_args, arg_txt);
            } else {
                char arg_txt[128];
                sprintf(arg_txt, "%s %s", info.tipo_llvm_str, info.operando);
                strcat(buffer_args, arg_txt);
            }
        }
        char ret_reg[16];
        sprintf(ret_reg, "%%%d", contador_temp++);
        fprintf(out_file, "    %s = call %s @%s(%s)\n",
                ret_reg,
                ll_tipo_pascal_para_llvm(proc->tipo),  // lembre-se de ter `proc` no escopo correto
                nome,
                buffer_args);
        printf(    "    %s = call %s @%s(%s)\n",
                   ret_reg,
                   ll_tipo_pascal_para_llvm(proc->tipo),
                   nome,
                   buffer_args);
    }
}