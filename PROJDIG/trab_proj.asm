MAT1: .word 1 2 2 2 1 2 2 2 1 # cada posicao eh 4 bytes

trab:
	la $s0, MAT1 # load adress de MAT1 para o registrador $s0
	li $s1, 1    # linha(row) contador - Cada 4 bytes   - (row-1) * 20
	li $s2, 2    # coluna(col) contador - Cada 20 bytes - (col-1) * 4
	li $t6, 3    # load numero de linhas e colunas
sla:
	addiu