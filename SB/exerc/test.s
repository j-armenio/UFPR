.section .data
DATA_ITEMS : .quad 3 , 67 , 34 , 250 , 45 , 75 , 54 , 34 , 44 , 33 , 22 , 11 , 66 , 0

.section .text
.globl _start
_start:
    movq $0, %rdi
    movq DATA_ITEMS(, %rdi, 8), %rbx
    movq $1, %rdi

loop:
    movq DATA_ITEMS(, %rdi, 8), %rax
    cmpq $0, %rax
    je fim_loop
    cmpq %rbx, %rax
    jle fim_if
    movq %rax, %rbx

fim_if:
    addq $1, %rdi
    jmp loop

fim_loop:
    movq %rbx, %rdi
    movq $60, %rax
    syscall
