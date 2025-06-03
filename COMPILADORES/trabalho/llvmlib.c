#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llvmlib.h"

int contador_temp = 0;
int contador_rotulo = 0;

// Imprime informações da struct infoLLVM
void imprime_infoLLVM(struct infoLLVM info)
{
    printf("infoLLVM: oper=%s, tipo=%s\n", info.operando, info.tipo_llvm_str);
}

// Empilha info na lista l
struct lista_info *ll_insere_info_lista(struct lista_info *l, struct infoLLVM info)
{
    struct lista_info *novo = malloc(sizeof(*novo));
    if (!novo){
        printf("Erro de alocação em ll_insere_info_lista\n");
        exit(1);
    }
    novo->info = info;
    novo->proximo = l;
    return novo;
}

// Converte o tipo do Pascal para a string correspondente do LLVM
const char *ll_tipo_pascal_para_llvm(Tipo t) 
{
    switch (t) {
        case INTEIRO: return "i32";
        case REAL: return "double";
        default: return "void";
    }
}

// Cria um registrador com id único
char *ll_novo_temporario() 
{
    char buffer[20];
    sprintf(buffer, "%%%d", contador_temp++);
    return strdup(buffer);
}
void ll_resetar_contador_temporario() { contador_temp = 0; }

char *ll_novo_rotulo()
{
    char buffer[20];
    sprintf(buffer, "l%d", contador_rotulo++);
    return strdup(buffer);
}

// Escreve as funções nativas no arquivo de saída
void ll_insere_funcoes_nativas(FILE *out_file)
{
    fprintf(out_file, "declare i32 @printf(ptr noundef, ...)\n");
    fprintf(out_file, "declare i32 @scanf(ptr noundef, ...)\n");
    fprintf(out_file, "@read_int = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
    fprintf(out_file, "@read_float = private unnamed_addr constant [4 x i8] c\"%%f\\00\", align 1\n");
    fprintf(out_file, "@write_int = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n");
    fprintf(out_file, "@write_float = private unnamed_addr constant [4 x i8] c\"%%f\\00\", align 1\n");
    fprintf(out_file, "\n");
}

/* Escreve em file a chamada de procedimento do usuário  */
void ll_escreve_chamada_proc(
    FILE *out_file, 
    struct tabela_simbolos *tab_simbolos, 
    const char *nome, 
    struct lista_info *args,
    struct simbolo* proc)
{
    if (strcmp(nome, "read") == 0 || strcmp(nome, "readln") == 0) {
        for (struct lista_info *it = args; it; it = it->proximo) {
            struct infoLLVM info = it->info;
            if (!info.var_name)
                printf("read exige IDENTIFICADOR puro");
            
            struct simbolo *s = busca_simbolo(tab_simbolos, info.var_name);
            if (!s)
                printf("variavel nao declarada em read()");
            
            char ptr_dest[64];
            if (s->escopo == 0) // global
                sprintf(ptr_dest, "@%s", s->lexema);
            else // local
                sprintf(ptr_dest, "%%%s", s->lexema);

            char *temp = ll_novo_temporario();
            const char *fmt = (s->tipo == REAL ? "@read_float" : "@read_int");

            fprintf(out_file, "    %s = call i32 (ptr, ...) @scanf(ptr %s, ptr %s)\n",
                temp, fmt, ptr_dest);
            printf("    %s = call i32 (ptr, ...) @scanf(ptr %s, ptr %s)\n",
                temp, fmt, ptr_dest);
        }
    }
    else if (strcmp(nome, "write") == 0 || strcmp(nome, "writeln") == 0) {
        for (struct lista_info *it = args; it; it = it->proximo) {
            struct infoLLVM info = it->info;
            if (info.var_name) { // argumento eh variavel
                struct simbolo *s = busca_simbolo(tab_simbolos, info.var_name);
                if (!s)
                    printf("variavel nao declarada em write()");
    
                /* carrega valor de @x/%x num temp */
                char var_addr[64];
                if (s->escopo == 0)
                    sprintf(var_addr, "@%s", s->lexema);
                else
                    sprintf(var_addr, "%%%s", s->lexema);

                char *temp_val = ll_novo_temporario();
                fprintf(out_file, "    %s = load %s, ptr %s\n",
                    temp_val,
                    ll_tipo_pascal_para_llvm(s->tipo),
                    var_addr);
                printf("    %s = load %s, ptr %s\n",
                    temp_val,
                    ll_tipo_pascal_para_llvm(s->tipo),
                    var_addr);
                
                char *temp_print = ll_novo_temporario();
                const char *fmt = (s->tipo == REAL ? "@write_float" : "@write_int");

                fprintf(out_file, "    %s = call i32 (ptr, ...) @printf(ptr %s, %s %s)\n",
                    temp_print,
                    fmt,
                    ll_tipo_pascal_para_llvm(s->tipo),
                    temp_val);
                printf("    %s = call i32 (ptr, ...) @printf(ptr %s, %s %s)\n",
                    temp_print,
                    fmt,
                    ll_tipo_pascal_para_llvm(s->tipo),
                    temp_val);
            } else { // argumento eh expressao ou literal
                char *temp = ll_novo_temporario();
                const char *fmt = (info.tipo_pascal == REAL ? "@write_float" : "@write_int");

                fprintf(out_file, "    %s = call i32 (ptr, ...) @printf(ptr %s, %s %s)\n",
                    temp, 
                    fmt, 
                    info.tipo_llvm_str, 
                    info.operando);
                printf("    %s = call i32 (ptr, ...) @printf(ptr %s, %s %s)\n",
                    temp, fmt, info.tipo_llvm_str, info.operando);
            }
        }
    }
    else { // chamada de proc do usuario
        // monta "%n = call <tipo> @nome(<lista_args>)"
        char buffer_args[512] = "";
        int primeiro = 1;

        // Inverte a lista de argumentos para impressao
        struct lista_info *rev = NULL, *tmp;
        for (struct lista_info *it = args; it; it = it->proximo){
            tmp = malloc(sizeof(*tmp));
            tmp->info = it->info;
            tmp->proximo = rev;
            rev = tmp;
        }
        args = rev;

        for (struct lista_info *it = args; it; it = it->proximo) {
            if (!primeiro)
                strcat(buffer_args, ", ");
            primeiro = 0;

            struct infoLLVM info = it->info;
            if (info.var_name) {
                // passar i32 @x ou double %y
                struct simbolo *s = busca_simbolo(tab_simbolos, info.var_name);
                if (!s)
                    printf("variavel nao declarada em chamada de funcao");
 
                /* Carregar num temp */
                char *temp_arg = ll_novo_temporario();
                fprintf(out_file, "    %s = load %s, ptr %%%s\n",
                    temp_arg,
                    ll_tipo_pascal_para_llvm(s->tipo),
                    s->lexema);
                printf("    %s = load %s, ptr %%%s\n",
                    temp_arg,
                    ll_tipo_pascal_para_llvm(s->tipo),
                    s->lexema);

                
                const char *tipo_str = ll_tipo_pascal_para_llvm(s->tipo);
                char arg_txt[128];
                if (s->escopo == 0)
                    sprintf(arg_txt, "%s @%s", tipo_str, s->lexema);
                else 
                    sprintf(arg_txt, "%s %%%s", tipo_str, s->lexema);
                strcat(buffer_args, arg_txt);
            } else {
                // passar i32 %n ou double %m ou literal i32 42
                char arg_txt[128];
                sprintf(arg_txt, "%s %s", info.tipo_llvm_str, info.operando);
                strcat(buffer_args, arg_txt);
            }
        }

        // gera registrador para retorno
        char ret_reg[16];
        sprintf(ret_reg, "%%%d", contador_temp++);

        fprintf(out_file, "    %s = call %s @%s(%s)\n",
            ret_reg,
            ll_tipo_pascal_para_llvm(proc->tipo),
            nome,
            buffer_args);
        printf("    %s = call %s @%s(%s)\n",
            ret_reg,
            ll_tipo_pascal_para_llvm(proc->tipo),
            nome,
            buffer_args);
    }
}

struct infoLLVM ll_operacao_binaria(
    FILE *out_file,
    struct tabela_simbolos *tab_simbolos,
    struct infoLLVM op1_info,
    struct infoLLVM op2_info,
    const char *op_pascal) // '+' ou '-'
{
        struct infoLLVM resultado_info;

        if (op1_info.tipo_pascal == REAL || op2_info.tipo_pascal == REAL){
            resultado_info.tipo_pascal = REAL;
        } else {
            resultado_info.tipo_pascal = INTEIRO;
        }
        resultado_info.tipo_llvm_str = strdup(ll_tipo_pascal_para_llvm(resultado_info.tipo_pascal));

        char *op1_final = op1_info.operando;
        char *op2_final = op2_info.operando;

        /* carrega variaveis se for necessario */
        if (op1_info.operando == NULL) {
            op1_final = ll_novo_temporario();

            struct simbolo *s = busca_simbolo(tab_simbolos, op1_info.var_name);
            char addr1[64];
            if (s->escopo == 0)
                sprintf(addr1, "@%s", s->lexema);
            else
                sprintf(addr1, "%%%s", s->lexema);

            fprintf(out_file, "    %s = load %s, ptr %s\n",
                op1_final, 
                op1_info.tipo_llvm_str, 
                addr1);
            printf("    %s = load %s, ptr %s\n",
                op1_final, op1_info.tipo_llvm_str, addr1);
        } else {
            op1_final = op1_info.operando;
        }
        if (op2_info.operando == NULL) {
            op2_final = ll_novo_temporario();

            struct simbolo *s = busca_simbolo(tab_simbolos, op2_info.var_name);
            char addr2[64];
            if (s->escopo == 0)
                sprintf(addr2, "@%s", s->lexema);
            else
                sprintf(addr2, "%%%s", s->lexema);

            fprintf(out_file, "    %s = load %s, ptr %s\n",
                op2_final, 
                op2_info.tipo_llvm_str,
                addr2);
            printf("    %s = load %s, ptr %s\n",
                op2_final, op2_info.tipo_llvm_str, addr2);
        } else {
            op2_final = op2_info.operando;
        }

        // se for REAL, converter operandos que vieram como i32 -> double
        const char *instrucao;
        if (resultado_info.tipo_pascal == REAL) {
            // instruções de ponto flutuante
            instrucao = (strcmp(op_pascal, "+") == 0 ? "fadd" : "fsub");

            if (strcmp(op1_info.tipo_llvm_str, "i32") == 0) {
                char *tmp = ll_novo_temporario();
                fprintf(out_file,
                    "    %s = sitofp i32 %s to double\n",
                    tmp,
                    op1_final
                );
                op1_final = tmp;
            }
            if (strcmp(op2_info.tipo_llvm_str, "i32") == 0) {
                char *tmp = ll_novo_temporario();
                fprintf(out_file,
                    "    %s = sitofp i32 %s to double\n",
                    tmp,
                    op2_final
                );
                op2_final = tmp;
            }
        } else {
            // instruções inteiras
            instrucao = (strcmp(op_pascal, "+") == 0 ? "add" : "sub");
            if (strcmp(op1_info.tipo_llvm_str, "double") == 0 ||
                strcmp(op2_info.tipo_llvm_str, "double") == 0) {
                printf("Erro semântico: operação inteira com operando REAL");
            }
        }

        resultado_info.operando = ll_novo_temporario();

        fprintf(out_file, "    %s = %s %s %s, %s\n",
            resultado_info.operando,
            instrucao,
            resultado_info.tipo_llvm_str,
            op1_final,
            op2_final);
        printf("    %s = %s %s %s, %s\n", resultado_info.operando, instrucao, resultado_info.tipo_llvm_str, op1_final, op2_final);

        if (op1_final != op1_info.operando)
            free(op1_final);
        if (op2_final != op2_info.operando)
            free(op2_final);

    return resultado_info;
}

