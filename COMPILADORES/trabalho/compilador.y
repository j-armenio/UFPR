/* BISON analisador sintático/parser */

/* Conceitos aleatórios importantes

EXPRESSAO
    Pode conter comparações (>, <, =, etc)
    Armazena: INTEIRO
    Usado para: avaliar condição booleana
EXPRESSAO_SIMPLES
    Pode conter soma, subtração, OR
    Armazena: o tipo resultante da expressão (INTEIRO, REAL)
    Usando para: avaliar expressoes matemáticas (não comparações)
TERMO
    Pode conter multiplicação, divisão, AND
    Armazena: o tipo do resultado da operação entre FATORES
FATOR
    Elementos mais básicos (números, variáveis, chamadas de função)
    Armazena: tipo do valor (INTEIRO, REAL)

*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"
#include "llvmlib.h"
#include "compilador.h"

int yylex();
extern FILE *yyin;
extern int yylineno;
FILE *log_file, *out_file;

struct tabela_simbolos *tab_simbolos = NULL;
int escopo_atual = 0;
extern int contador_temp;

char *then_label;
char *else_label;
char *end_label;

char *while_cond_label;
char *while_body_label;
char *while_end_label;
%}

%define parse.error verbose

%union {
    char *lexema;
    Tipo tipo; 
    struct lista_simbolo *lista_s;
    struct infoLLVM info_llvm;
    struct lista_info *lista_i;
    struct {
        char *then_label;
        char *else_label;
        char *end_label;
    } if_info;
}

/* Declaração dos Tokens */
%token PROGRAM VAR TK_INTEIRO TK_REAL FUNCTION PROCEDURE BEGIN_TOKEN END IF THEN ELSE WHILE DO
%token OPERADOR_ATRIBUICAO
%token MAIS MENOS OR
%token ABRE_PARENTESES FECHA_PARENTESES
%token VIRGULA PONTO_VIRGULA DOIS_PONTOS PONTO_FINAL
%token <lexema> ID NUM OPERADOR_RELACIONAL OPERADOR_MULTIPLICATIVO

%type <tipo> TIPO CHAMADA_DE_PROCEDIMENTO
%type <lista_s> LISTA_DE_IDENTIFICADORES ARGUMENTOS LISTA_DE_PARAMETROS
%type <info_llvm> FATOR TERMO EXPRESSAO_SIMPLES EXPRESSAO VARIAVEL
%type <lista_i> LISTA_DE_EXPRESSOES
%type <lexema> CABECALHO_DE_SUBPROGRAMA

%left OR
%left MAIS MENOS
%left OPERADOR_MULTIPLICATIVO

%%


PROGRAMA:
    PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES PONTO_VIRGULA {
        /* insere os parametros de entrada do programa */
        tab_simbolos = insere_simbolos_ts(tab_simbolos, $4);
    }
    DECLARACOES
    DECLARACOES_DE_SUBPROGRAMAS {
        fprintf(out_file, "define i32 @main() {\nentry:\n");
        printf("define i32 @main() {\nentry:\n");
        ll_resetar_contador_temporario();
    }
    ENUNCIADO_COMPOSTO
    PONTO_FINAL {
        fprintf(out_file, "    ret i32 0\n}\n");
        printf("    ret i32 0\n}\n");
        //imprime_tabela_simbolos(stdout, tab_simbolos);
    }
    ;

LISTA_DE_IDENTIFICADORES: 
    ID {
        $$ = insere_lista_simbolo(NULL, novo_simbolo3($1, VARIAVEL, escopo_atual));
    }
    | LISTA_DE_IDENTIFICADORES VIRGULA ID {
        $$ = insere_lista_simbolo($1, novo_simbolo3($3, VARIAVEL, escopo_atual));
    }
    ;

DECLARACOES: 
    DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA {
        atualiza_tipo_simbolos($3, $5); // atribui tipo a lista de ids

        /* Escreve as variaveis globais no LLVM */
        struct lista_simbolo *l = $3;
        while (l) {
            struct simbolo *s = l->simb;
            if (s->escopo == 0) {
                char *valor_inicial = (s->tipo == INTEIRO) ? "0" : "0.0";

                fprintf(out_file, "@%s = global %s %s\n", s->lexema, ll_tipo_pascal_para_llvm(s->tipo), valor_inicial);
                printf("@%s = global %s %s\n", s->lexema, ll_tipo_pascal_para_llvm(s->tipo), valor_inicial);
            }
            l = l->proximo;
        }
        fprintf(out_file, "\n");
        printf("\n");

        tab_simbolos = insere_simbolos_ts(tab_simbolos, $3); // insere na tabela
    }
    | /* empty */ 
    ;

TIPO: 
    TK_INTEIRO { $$ = INTEIRO; }
    | TK_REAL  { $$ = REAL; }
    ;

DECLARACOES_DE_SUBPROGRAMAS: 
    DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA PONTO_VIRGULA 
    | /* empty */
    ;

DECLARACAO_DE_SUBPROGRAMA: 
    CABECALHO_DE_SUBPROGRAMA DECLARACOES ENUNCIADO_COMPOSTO {
        /* "fechamento" da função */
        struct simbolo *f = busca_simbolo(tab_simbolos, $1);
        char *ret_reg = ll_novo_temporario();

        if (f->tipo != VAZIO) {
            fprintf(out_file, "    %s = load %s, ptr %%%s\n",
                ret_reg, ll_tipo_pascal_para_llvm(f->tipo), f->lexema);
            printf("    %s = load %s, ptr %%%s\n",
                ret_reg, ll_tipo_pascal_para_llvm(f->tipo), f->lexema);
            
            fprintf(out_file, "    ret %s %s\n",
                ll_tipo_pascal_para_llvm(f->tipo), ret_reg);
            printf("    ret %s %s\n",
                ll_tipo_pascal_para_llvm(f->tipo), ret_reg);
        } else {
            fprintf(out_file, "    ret void\n");
            printf("    ret void\n");
        }

        fprintf(out_file, "}\n\n");
        printf("}\n\n");

        tab_simbolos = remove_simbolos(tab_simbolos, escopo_atual);
        escopo_atual--;
    }
    ;

CABECALHO_DE_SUBPROGRAMA: 
    FUNCTION ID ARGUMENTOS DOIS_PONTOS TIPO PONTO_VIRGULA {
        struct simbolo *func = novo_simbolo4($2, FUNCAO, escopo_atual, $5);
        tab_simbolos = insere_simbolo_ts(tab_simbolos, func);
        escopo_atual++;
        
        int n_params = 0;
        for (struct lista_simbolo *p = $3; p; p = p->proximo)
            n_params++;
        contador_temp = n_params;

        /* Cabeçalho LLVM */
        fprintf(out_file, "define %s @%s(", ll_tipo_pascal_para_llvm($5), $2);
        printf("define %s @%s(", ll_tipo_pascal_para_llvm($5), $2);

        /* Escreve parametros LLVM */
        struct lista_simbolo *param = $3;
        int pos = 0;
        while (param) {
            if (pos > 0) {
                fprintf(out_file, ", ");
                printf(", ");
            }
            fprintf(out_file, "%s %%%d", ll_tipo_pascal_para_llvm(param->simb->tipo), pos);
            printf("%s %%%d", ll_tipo_pascal_para_llvm(param->simb->tipo), pos);
            pos++;
            param = param->proximo;
        }
        fprintf(out_file, ") {\nentry:\n");
        printf(") {\nentry:\n");

        /* para cada parametro, aloca local e armazena valor recebido */
        param = $3;
        int idx = 0;
        while (param) {
            const char *llvm_tipo = ll_tipo_pascal_para_llvm(param->simb->tipo);
            fprintf(out_file, "    %%p%d = alloca %s\n", idx, llvm_tipo);
            printf("    %%p%d = alloca %s\n", idx, llvm_tipo);
            fprintf(out_file, "    store %s %%%d, ptr %%p%d\n",
                    llvm_tipo, idx, idx);
            printf("    store %s %%%d, ptr %%p%d\n", llvm_tipo, idx, idx);
            idx++;
            param = param->proximo;
        }

        /* aloca variavel de retorno */
        fprintf(out_file, "    %%%s = alloca %s\n", $2, ll_tipo_pascal_para_llvm($5));
        printf("    %%%s = alloca %s\n", $2, ll_tipo_pascal_para_llvm($5));

        /* Cria variável de retorno implicita: mesmo nome, tipo de retorno */
        struct simbolo *ret_var = novo_simbolo3($2, VARIAVEL, escopo_atual);
        ret_var->tipo = $5; // tipo de retorno
        tab_simbolos = insere_simbolo_ts(tab_simbolos, ret_var);

        /* Associa os parametros a função e os insere na TS (no escopo da função) */
        atualiza_escopo_simbolos($3, escopo_atual);
        insere_func_args(func, $3);
        tab_simbolos = insere_simbolos_ts(tab_simbolos, $3);

        $$ = $2;
    }
    | PROCEDURE ID ARGUMENTOS PONTO_VIRGULA {
        struct simbolo *proc = novo_simbolo3($2, FUNCAO, escopo_atual);
        proc->tipo = VAZIO;
        tab_simbolos = insere_simbolo_ts(tab_simbolos, proc);
        escopo_atual++;

        int n_params = 0;
        for (struct lista_simbolo *p = $3; p; p = p->proximo)
            n_params++;
        contador_temp = n_params;

        /* Cabeçalho LLVM */
        fprintf(out_file, "define void @%s(", $2);
        printf("define void @%s(", $2);

        /* Escreve parametros LLVM */
        struct lista_simbolo *param = $3;
        int pos = 0;
        while (param) {
            if (pos > 0) {
                fprintf(out_file, ", ");
                printf(", ");
            }
            fprintf(out_file, "%s %%%d", ll_tipo_pascal_para_llvm(param->simb->tipo), pos);
            printf("%s %%%d", ll_tipo_pascal_para_llvm(param->simb->tipo), pos);
            pos++;
            param = param->proximo;
        }
        fprintf(out_file, ") {\nentry:\n");
        printf(") {\nentry:\n");

        atualiza_escopo_simbolos($3, escopo_atual);
        insere_func_args(proc, $3);
        tab_simbolos = insere_simbolos_ts(tab_simbolos, $3);

        $$ = $2;
    }
    ;

ARGUMENTOS:
    ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES {
        $$ = $2; /* retorna so os parametros msm */
    }
    | /* empty */ {
        $$ = NULL;
    }
    ;

LISTA_DE_PARAMETROS:
    LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        atualiza_tipo_simbolos($1, $3);
        $$ = $1;
    }
    | VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        atualiza_tipo_simbolos($2, $4);
        $$ = $2;
    }
    | LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        atualiza_tipo_simbolos($3, $5);
        $$ = concatena_listas_simbolos($1, $3);
    }
    | LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO {
        atualiza_tipo_simbolos($4, $6);
        $$ = concatena_listas_simbolos($1, $4);
    }
    ;

ENUNCIADO_COMPOSTO: 
    BEGIN_TOKEN ENUNCIADOS_OPCIONAIS END
    ;

ENUNCIADOS_OPCIONAIS: 
    LISTA_DE_ENUNCIADOS
    | /* empty */
    ;

LISTA_DE_ENUNCIADOS: 
    ENUNCIADO
    | LISTA_DE_ENUNCIADOS PONTO_VIRGULA ENUNCIADO
    ;

ENUNCIADO:
    VARIAVEL OPERADOR_ATRIBUICAO EXPRESSAO { /* atribuição: VARIAVEL := EXPRESSAO */
        struct simbolo *s = busca_simbolo(tab_simbolos, $1.operando);

        if (s->tipo == INTEIRO && $3.tipo_pascal == REAL)
            yyerror("não é permitido atribuir real a inteiro");

        /* gera o store do valor em %3 */
        char ptr_dest[256];
        if (s->escopo == 0){ // variavel global
            sprintf(ptr_dest, "@%s", s->lexema);
        } else { // variavel local
            sprintf(ptr_dest, "%%%s", s->lexema);
        }
        fprintf(out_file, "    store %s %s, ptr %s\n",
            $3.tipo_llvm_str,
            $3.operando,
            ptr_dest);
        printf("    store %s %s, ptr %s\n", $3.tipo_llvm_str, $3.operando, ptr_dest);
    }
    | CHAMADA_DE_PROCEDIMENTO
    | ENUNCIADO_COMPOSTO
    | IF EXPRESSAO THEN {
        then_label = ll_novo_rotulo();
        else_label = ll_novo_rotulo();
        end_label = ll_novo_rotulo();

        // salto condicional
        fprintf(out_file, "    br i1 %s, label %%%s, label %%%s\n\n",
            $2.operando, then_label, else_label);
        printf("    br i1 %s, label %%%s, label %%%s\n\n",
            $2.operando, then_label, else_label);

        // ja abre rotulo "then"
        fprintf(out_file, "%s:\n", then_label);
        printf("%s:\n", then_label);
    } 
    ENUNCIADO {
        // pos-corpo do then gerado
        fprintf(out_file, "    br label %%%s\n\n", end_label);
        printf("    br label %%%s\n\n", end_label);

        // abre rotulo "else"
        fprintf(out_file, "%s:\n", else_label);
        printf("%s:\n", else_label);
    } 
    ELSE ENUNCIADO {
        // pos-corpo do else gerado
        fprintf(out_file, "    br label %%%s\n\n", end_label);
        printf("    br label %%%s\n\n", end_label);

        // abre rotulo "end"
        fprintf(out_file, "%s:\n", end_label);
        printf("%s:\n", end_label);
    }
    | WHILE {
        // registra rotulos
        while_cond_label = ll_novo_rotulo();
        while_body_label = ll_novo_rotulo();
        while_end_label = ll_novo_rotulo();

        // salto incondicional
        fprintf(out_file, "    br label %%%s\n\n", while_cond_label);
        printf("    br label %%%s\n\n", while_cond_label);

        // rotulo de condicao
        fprintf(out_file, "%s:\n", while_cond_label);
        printf("%s:\n", while_cond_label);
    } 
    EXPRESSAO {
        // grava resultado da EXPRESSAO numa variavel local
        // n entendi direito pq, mas se tentar acessar direto n compila
        struct infoLLVM cond_info = $3;

        // se true -> loop_body ; senao loop_end
        fprintf(out_file, "    br i1 %s, label %%%s, label %%%s\n\n",
            cond_info.operando,
            while_body_label,
            while_end_label);
        printf("    br i1 %s, label %%%s, label %%%s\n\n",
            cond_info.operando,
            while_body_label,
            while_end_label);

        // rotulo do corpo
        fprintf(out_file, "%s:\n", while_body_label);
        printf("%s:\n", while_body_label);
    } 
    DO ENUNCIADO {
        // código foi gerado, volta para condicao
        fprintf(out_file, "    br label %%%s\n\n", while_cond_label);
        printf("    br label %%%s\n\n", while_cond_label);

        // abre rotulo de saida
        fprintf(out_file, "%s:\n", while_end_label);
        printf("%s:\n", while_end_label);
    }
    ;

VARIAVEL: 
    ID {
        struct simbolo *s = busca_simbolo(tab_simbolos, $1);
        if (!s || s->tipo_simb != VARIAVEL)
            yyerror("variável não declarada");

        struct infoLLVM var_i;
        var_i.operando = strdup($1);
        var_i.tipo_pascal = s->tipo;
        var_i.tipo_llvm_str = strdup(ll_tipo_pascal_para_llvm(s->tipo));
        var_i.var_name = NULL;

        $$ = var_i;
    }
    ;

CHAMADA_DE_PROCEDIMENTO: 
    ID { /* proc sem nada */
        struct simbolo *proc = busca_funcao(tab_simbolos, $1);
        if (!proc || proc->tipo_simb != FUNCAO)
            yyerror("procedimento não declarado");
        if (proc->args)
            yyerror("assinatura de procedimento invalida");

        $$ = proc->tipo;
    }
    | ID ABRE_PARENTESES FECHA_PARENTESES { /* proc só com parenteses */
        struct simbolo *proc = busca_funcao(tab_simbolos, $1);
        if (!proc || proc->tipo_simb != FUNCAO)
            yyerror("procedimento não declarado");
        if (proc->args)
            yyerror("assinatura de procedimento invalida");

        $$ = proc->tipo;
    }
    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES { /* proc com args */
        struct simbolo *proc = busca_funcao(tab_simbolos, $1);
        if (!proc || proc->tipo_simb != FUNCAO)
            yyerror("procedimento não declarada");
        
        // se for procedimento (ret VAZIO), cada arg->info.var_name
        if (proc->tipo == VAZIO) {
            for (struct lista_info *it = $3; it; it = it->proximo) {
                if (it->info.var_name == NULL)
                    yyerror("procedure exige variavel simples, nao expressao");
                // verifica se existe na ts
                if (!busca_simbolo(tab_simbolos, it->info.var_name))
                    yyerror("variavel nao declarada em chamada de procedimentos");
            }
        }
        
        /* Gera call do procedimento com operandos em 3 */
        ll_escreve_chamada_proc(out_file, tab_simbolos, $1, $3, proc);
        $$ = proc->tipo;
    }
    ;

LISTA_DE_EXPRESSOES: 
    EXPRESSAO {
        $$ = ll_insere_info_lista(NULL, $1);
    }
    | LISTA_DE_EXPRESSOES VIRGULA EXPRESSAO {
        $$ = ll_insere_info_lista($1, $3);
    }
    ;

EXPRESSAO: 
    EXPRESSAO_SIMPLES {
        $$ = $1;
    }
    | EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES {
        struct infoLLVM op1_info = $1;
        struct infoLLVM op2_info = $3;
        struct infoLLVM resultado_info;
        char *op1_final, *op2_final;
        const char *predicado_llvm;
        const char *instrucao_cmp = "icmp";

        /* resultado de uma comparação é sempre i1 */
        resultado_info.tipo_pascal   = INTEIRO;
        resultado_info.tipo_llvm_str = strdup("i1");

        /* se vier de variavel, carrega op1 */
        if (op1_info.operando == NULL) {
            struct simbolo *s1 = busca_simbolo(tab_simbolos, op1_info.var_name);
            if (!s1)
                yyerror("variavel nao declarada em comparacao");
            
            char ptr1[64];
            if (s1->escopo == 0)
                sprintf(ptr1, "@%s", s1->lexema);
            else 
                sprintf(ptr1, "%%%s", s1->lexema);

            /* op1 vinha de uma variáve */
            op1_final = ll_novo_temporario();
            
            fprintf(out_file, "    %s = load %s, ptr %s\n",
                op1_final,
                op1_info.tipo_llvm_str,
                ptr1
            );
            printf("    %s = load %s, ptr %s\n",
                op1_final,
                op1_info.tipo_llvm_str,
                ptr1
            );
        } else {
            op1_final = op1_info.operando;
        }

        /* se vier de variavel, carrega op2 */
        if (op2_info.operando == NULL) {
            op2_final = ll_novo_temporario();
            fprintf(out_file, "    %s = load %s, ptr %%%s\n",
                op2_final,
                op2_info.tipo_llvm_str,
                op2_info.var_name
            );
            printf("    %s = load %s, ptr %%%s\n",
                op2_final,
                op2_info.tipo_llvm_str,
                op2_info.var_name
            );
        } else {
            op2_final = op2_info.operando;
        }

        /* decide predicado_llvm com base em 2 (op_pascal) */
        if (strcmp(op1_info.tipo_llvm_str, "double") == 0 ||
            strcmp(op2_info.tipo_llvm_str, "double") == 0)
        {
            instrucao_cmp = "fcmp";
            if (strcmp($2, "=")  == 0) predicado_llvm = "oeq";
            if (strcmp($2, "<>") == 0) predicado_llvm = "one";
            if (strcmp($2, "<")  == 0) predicado_llvm = "olt";
            if (strcmp($2, "<=") == 0) predicado_llvm = "ole";
            if (strcmp($2, ">")  == 0) predicado_llvm = "ogt";
            if (strcmp($2, ">=") == 0) predicado_llvm = "oge";
        } else {
            /* comparação inteira */
            instrucao_cmp = "icmp"; 
            if (strcmp($2, "=")  == 0) predicado_llvm = "eq";
            if (strcmp($2, "<>") == 0) predicado_llvm = "ne";
            if (strcmp($2, "<")  == 0) predicado_llvm = "slt";
            if (strcmp($2, "<=") == 0) predicado_llvm = "sle";
            if (strcmp($2, ">")  == 0) predicado_llvm = "sgt";
            if (strcmp($2, ">=") == 0) predicado_llvm = "sge";
        }

        resultado_info.operando      = ll_novo_temporario();

        /* imprime “icmp/ fcmp” com os operandos já carregados —— */
        fprintf(out_file, "    %s = %s %s %s %s, %s\n",
            resultado_info.operando,
            instrucao_cmp,                
            predicado_llvm,
            op1_info.tipo_llvm_str,
            op1_final,                     
            op2_final                      
        );
        printf("    %s = %s %s %s %s, %s\n",
            resultado_info.operando,
            instrucao_cmp,
            predicado_llvm,
            op1_info.tipo_llvm_str,
            op1_final,
            op2_final
        );

        if (op1_final != op1_info.operando) free(op1_final);
        if (op2_final != op2_info.operando) free(op2_final);

        $$ = resultado_info;
    }
    ;

EXPRESSAO_SIMPLES:
    TERMO {
        $$ = $1;
    }
    | SINAL TERMO {
        $$ = $2;
    } 
    | EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES {
        $$ = ll_operacao_binaria(out_file, tab_simbolos, $1, $3, "+");

    } 
    | EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES {
        $$ = ll_operacao_binaria(out_file, tab_simbolos, $1, $3, "-");
    }
    | EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES {
        struct infoLLVM op1_info = $1;
        struct infoLLVM op2_info = $3;
        struct infoLLVM resultado_info;

        if (op1_info.tipo_pascal != INTEIRO || op2_info.tipo_pascal != INTEIRO) {
            yyerror("Operador 'or' requer operandos do tipo Inteiro (booleano).");
        }
        resultado_info.tipo_pascal = INTEIRO;
        resultado_info.tipo_llvm_str = strdup(ll_tipo_pascal_para_llvm(INTEIRO));

        resultado_info.operando = ll_novo_temporario();

        if (strcmp(op1_info.tipo_llvm_str, "i32") != 0 || strcmp(op2_info.tipo_llvm_str, "i32") != 0) {
            yyerror("Erro interno: 'or' com tipos LLVM inesperados para operandos Inteiro.");
        }

        fprintf(out_file, "    %s = or %s %s, %s\n",
                resultado_info.operando,
                resultado_info.tipo_llvm_str,
                op1_info.operando,
                op2_info.operando);
        printf("    %s = or %s %s, %s\n", resultado_info.operando, resultado_info.tipo_llvm_str, op1_info.operando, op2_info.operando);

        $$ = resultado_info;
    }
    ;

TERMO: 
    FATOR {
        $$ = $1;
    }
    | TERMO OPERADOR_MULTIPLICATIVO FATOR {
        struct infoLLVM op1_info = $1;
        struct infoLLVM op2_info = $3;
        struct infoLLVM resultado_info;

        const char* operador_str = $2;

        // Regras de tipo para Pascal:
        //  *: se for REAL, resultado é REAL. Senão, INTEIRO.
        //  /: sempre REAL.
        //  div, mod, and: sempre INTEIRO.

        if (strcmp(operador_str, "/") == 0) {
            resultado_info.tipo_pascal = REAL;
        } else if (strcmp(operador_str, "div") == 0 || strcmp(operador_str, "mod") == 0 || strcmp(operador_str, "and") == 0) {
            if (op1_info.tipo_pascal != INTEIRO || op2_info.tipo_pascal != INTEIRO) {
                yyerror("Operador requer operandos inteiros.");
            }
            resultado_info.tipo_pascal = INTEIRO;
        } else if (strcmp(operador_str, "*") == 0) {
            if (op1_info.tipo_pascal == REAL || op2_info.tipo_pascal == REAL) {
                resultado_info.tipo_pascal = REAL;
            } else {
                resultado_info.tipo_pascal = INTEIRO;
            }
        } else {
            yyerror("Operador multiplicativo desconhecido.");
        }

        resultado_info.tipo_llvm_str = strdup(ll_tipo_pascal_para_llvm(resultado_info.tipo_pascal));

        resultado_info.operando = ll_novo_temporario();

        char* op1_final = op1_info.operando;
        char* op2_final = op2_info.operando;
        const char* instrucao_llvm;

        // Conversões e escolha da instrução
        if (resultado_info.tipo_pascal == REAL) { // Resultado é REAL (para * ou /)
            if (strcmp(op1_info.tipo_llvm_str, "i32") == 0) {
                op1_final = ll_novo_temporario();
                fprintf(out_file, "    %s = sitofp i32 %s to double\n", op1_final, op1_info.operando);
                printf("    %s = sitofp i32 %s to double\n", op1_final, op1_info.operando);
            }
            if (strcmp(op2_info.tipo_llvm_str, "i32") == 0) {
                op2_final = ll_novo_temporario();
                fprintf(out_file, "    %s = sitofp i32 %s to double\n", op2_final, op2_info.operando);
                printf("    %s = sitofp i32 %s to double\n", op2_final, op2_info.operando);
            }

            if (strcmp(operador_str, "*") == 0) instrucao_llvm = "fmul";
            else if (strcmp(operador_str, "/") == 0) instrucao_llvm = "fdiv";
        } else { // Resultado é INTEIRO (para *, div, mod, and)
            if (strcmp(op1_info.tipo_llvm_str, "double") == 0 || strcmp(op2_info.tipo_llvm_str, "double") == 0)
                yyerror("Erro semântico: Operando REAL resultando em INTEIRO.");

            if (strcmp(operador_str, "*") == 0) instrucao_llvm = "mul";
            else if (strcmp(operador_str, "div") == 0) instrucao_llvm = "sdiv"; // Divisão inteira com sinal
            else if (strcmp(operador_str, "mod") == 0) instrucao_llvm = "srem"; // Resto com sinal
            else if (strcmp(operador_str, "and") == 0) instrucao_llvm = "and";  // AND bit-a-bit
        }

        // Gerar instrução LLVM
        fprintf(out_file, "    %s = %s %s %s, %s\n",
                resultado_info.operando,
                instrucao_llvm,
                resultado_info.tipo_llvm_str,
                op1_final,
                op2_final);
        printf("    %s = %s %s %s, %s\n", resultado_info.operando, instrucao_llvm, resultado_info.tipo_llvm_str, op1_final, op2_final);

        if (op1_final != op1_info.operando) free(op1_final);
        if (op2_final != op2_info.operando) free(op2_final);

        $$ = resultado_info;
    }
    ;

FATOR:
    ID { /* variáveis */
        struct simbolo *s = busca_simbolo(tab_simbolos, $1);
        if (!s)
            yyerror("identificador não declarado");

        struct infoLLVM id_info;
        id_info.tipo_pascal = s->tipo;
        id_info.tipo_llvm_str = strdup(ll_tipo_pascal_para_llvm(id_info.tipo_pascal));
        id_info.var_name = strdup($1);
        id_info.operando = NULL;

        $$ = id_info;
    }
    | ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES { /* chamada de função com parametros */    
        struct simbolo *f = busca_funcao(tab_simbolos, $1);
        if (!f || f->tipo_simb != FUNCAO)
            yyerror("função não declarada");
    
        struct lista_info *args = $3, *rev = NULL, *aux;
        for (struct lista_info *it = $3; it; it = it->proximo) {
            aux = malloc(sizeof(*aux));
            aux->info = it->info;
            aux->proximo = rev;
            rev = aux;
        }
        args = rev;

        // Verificação de Argumentos
        /* conta quantos parametros a funcao espera */ 
        int n_args=0;
        struct lista_args *la = f->args;
        while (la) {
            n_args++;
            la = la->proximo;
        }
        /* calcula numero de argumentos da LISTA_DE_EXPRESSOES */
        int n_args_le=0;
        struct lista_info *le = $3;
        while (le) {
            n_args_le++;
            le = le->proximo;
        } 
        if (n_args != n_args_le)
            yyerror("numero de argumentos incorreto");

        /* Verifica compatibilidade dos tipos */
        la = f->args;
        le = $3;
        while (la && le) {
            if (la->tipo != le->info.tipo_pascal)
                yyerror("incompatibilidade de tipos em chamada");
            la = la->proximo;
            le = le->proximo;
        }

        // Etapa de geração do código LLVM
        struct infoLLVM func_call_info;
        func_call_info.tipo_pascal = f->tipo;
        func_call_info.tipo_llvm_str = strdup(ll_tipo_pascal_para_llvm(f->tipo));
        func_call_info.var_name = NULL;

        char buffer_args[512] = "";
        int primeiro = 1;
        le = args;
        while (le) {
            if (!primeiro)
                strcat(buffer_args, ", ");
            primeiro = 0;

            char *valor_arg;
            if (le->info.operando == NULL) {
                // descobre se o identificador eh global ou local
                struct simbolo *s = busca_simbolo(tab_simbolos, le->info.var_name);
                if (!s)
                    yyerror("variavel nao declarada em chamada de funcao");
                
                char var_ptr[64];
                if (s->escopo == 0)
                    sprintf(var_ptr, "@%s", s->lexema);
                else
                    sprintf(var_ptr, "%%%s", s->lexema);

                valor_arg = ll_novo_temporario();

                fprintf(out_file, "    %s = load %s, ptr %s\n",
                    valor_arg,
                    le->info.tipo_llvm_str,
                    var_ptr);
                printf("    %s = load %s, ptr %s\n",
                    valor_arg,
                    le->info.tipo_llvm_str,
                    var_ptr);
            } else {
                valor_arg = le->info.operando;
            }

            char arg_txt[128];
            sprintf(arg_txt, "%s %s", 
                le->info.tipo_llvm_str, valor_arg);
            strcat(buffer_args, arg_txt);

            le = le->proximo;
        }

        char *ret_reg = ll_novo_temporario();
        func_call_info.operando = ret_reg;


        fprintf(out_file, "    %s = call %s @%s(%s)\n",
            ret_reg,
            func_call_info.tipo_llvm_str,
            $1,
            buffer_args);
        printf("    %s = call %s @%s(%s)\n",
            ret_reg,
            func_call_info.tipo_llvm_str,
            $1,
            buffer_args);
        
        $$ = func_call_info;
    }
    | NUM {
        struct infoLLVM num_info;

        if (strchr($1, '.') || strchr($1, 'E') || strchr($1, 'e'))
            num_info.tipo_pascal = REAL;
        else
            num_info.tipo_pascal = INTEIRO;

        num_info.operando = strdup($1);
        num_info.tipo_llvm_str = strdup(ll_tipo_pascal_para_llvm(num_info.tipo_pascal));
        num_info.var_name = NULL; // veio de numero

        $$ = num_info;

    }
    | ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES {
        $$ = $2;
    }
    ;

SINAL:
    MAIS
    | MENOS 
    ;


%%

// Se um arquivo é passado por linha de comando, ele é aberto como entrada
int main(int argc, char **argv) {
    log_file = fopen("compilador.log", "w");
    out_file = fopen("saida.ll", "w");
    if (!log_file || !out_file) {
        fprintf(stderr, "Erro ao abrir arquivos de log ou saída.\n");
        return 1;
    }

    tab_simbolos = adiciona_procedures_nativas(tab_simbolos);

    ll_insere_funcoes_nativas(out_file);

    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        yylineno=1;
        yyparse();
    } else if (argc == 1) {
        yyparse();
    }
    fprintf(log_file, "Finalizando compilacao\n");
    fclose(log_file);
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Erro na linha %d: %s\n", yylineno, s);
    exit(1);
}

