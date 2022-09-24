.data
msg1:	.asciiz "Entre com um termo para calcular: "
msg2:	.asciiz "O resultado eh: "
.text
	li $v0, 4
	la $a0, msg1
	syscall # imprimir a mensagem na tela
	li $v0, 5
	syscall # le um numero inteiro do teclado
	
	# chama funcao fib
	add $a0, $v0, $zero
	jal fib
	add $s0, $v0, $zero
	
	#imprime resultado na tela
	li $v0, 4
	la $a0, msg2
	syscall
	add $a0, $s0, $zero
	li $v0, 1
	syscall
	
	#termina a execucao
	li $v0, 10
	syscall
	
# fibonacci
# 1 1 2 3 5 8 13 21 34
#
# int fib(int n){
#     int i;
#     int n1 = 1, n2 = 1;
#     int prox;
#     for (i = 2; i < n; i++){
#         prox = n1 + n2;
#         n2 = n1;
#         n1 = prox;
#     }
#     return n1;
# }

.text
fib:	#i -> $t0; n1 -> $t1; n2 -> $t2; prox -> $t3; n -> $a0
	addi $t0, $zero, 2  # i = 2
	addi $t1, $zero, 1  # n1 = 1
	addi $t2, $zero, 1  # n2 = 1
for:
	bge $t0, $a0, fimFor
	add $t3, $t1, $t2   # prox = n1 + n2
	add $t2, $t1, $zero # n2 = n1
	add $t1, $t3, $zero # n1 = prox
	addi $t0, $t0, 1    # i++
	j for
fimFor:
	add $v0, $t1, $zero # return n1]
	jr $ra