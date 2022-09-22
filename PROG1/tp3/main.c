#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libpilha.h"

/*
    Exemplo:
    {(3 + 5) * (2 + 1)*1}/[3+1]
    CORRETA

    (3+2)*5}
    INCORRETA

    {3+2)*(3+}
    INCORRETA
*/

/* tamanho maximo da pilha */
#define TAMPICHAR 4096

int main(void)
{
	PILHA  *pi_expr; 
	char token, buffer[TAMPICHAR];
	/* int pos; */

	/* cria a pilha para a expressao */
	if ( (pi_expr = piCria(TAMPICHAR)) == NULL )
	{
		fprintf(stderr, "ERRO ALOCANDO PILHA CHAR");
		exit(-1);
	}

	/* le a expressao */
	printf("Entre a expressao\n"); 
	if ( fgets(buffer, TAMPICHAR, stdin) == NULL )
	{
		fprintf(stderr, "ERRO LENDO EXPRESSAO");
		exit(-1);
	} 

	/* strings tem o ultimo elemento \0 */
	int pos = 0;
	int erro = 0;
	char buf_atual = buffer[pos];

	while(buf_atual != "\0"){
		/* se for um simbolo que abre */
		if (buf_atual == "(" || buf_atual == "[" || buf_atual == "{"){
			piPush(pi_expr , buf_atual);
		}

		/* condicionais se for um simbolo que fecha */
		if (buf_atual == ")"){
			if (pi_expr->p_topo == "("){
				piPop(pi_expr);
			}
			else{
				printf("Expressão Inválida.");
				int erro = 1;
				break;
			}
		}
		if (buf_atual == "]"){
			if (pi_expr->p_topo == "["){
				piPop(pi_expr);
			}
			else{
				printf("Expressão Inválida.");
				int erro = 1;
				break;
			}
		}
		if (buf_atual == "}"){
			if (pi_expr->p_topo == "{"){
				piPop(pi_expr);
			}
			else{
				printf("Expressão Inválida.");
				int erro = 1;
				break;
			}
		}
		pos++;
	}

	/* verificar a ultima posicao */
	if (erro == 0){
		printf ("Operação Válida.");
	}

	piDestroi(pi_expr);

	return 0;
}
