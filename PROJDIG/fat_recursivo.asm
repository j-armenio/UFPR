.data
msg1:	.asciiz "Entre com um numero: "
msg2:	.asciiz "Fatorial do numero: "
.text
	li $v0, 4
	la $a0, msg1
	syscall
	# leio um numero inteiro do teclado
	li $v0, 5
	syscall
	
	# coloco como argumento para a funcao fat
	add $a0, $v0, $zero
	addi $a1, $zero, 1
	#chamo a funcao fat
	jal fat_rec
	add $s0, $v0, $zero
	
	li $v0, 4
	la $a0, msg2
	syscall
	# coloco o resultado de fat em a0
	add $a0, $s0, $zero
	li $v0, 1
	syscall
	
	# termina o programa
	li $v0, 10
	syscall

# fatorial recursivo 
# int fat_rec(int n, int i){
#     if(i == n)
#     	return n;
#     return i * fat_rec(n, i + 1);
# }

.text
fat_rec: # n -> $a0; i -> $a1
	bne $a0, $a1, continua
	add $v0, $a0, $zero
	jr $ra
continua:
	addi $sp, $sp, -8 # libera espaco para 2 palavras de 4 bytes na pilha
	sw $ra, 0($sp)    # salva ra na pilha
	sw $a1, 4($sp)    # salva i(a1) na pilha
	
	addi $a1, $a1, 1 # i = i + 1
	jal fat_rec
	
	lw $ra, 0 ($sp)   # carrega da pilha o valor de ra
	lw $a1, 4($sp)    # carrega da pilha o valor de i(a1)
	addi $sp, $sp, 8
	
	mul $t0, $a1, $v0
	add $v0, $t0, $zero
	jr $ra