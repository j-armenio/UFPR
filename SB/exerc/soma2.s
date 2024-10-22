# long int a, b;
# long int soma (long int x, long int y) {
#     long int z;
#     z = x + y;
#     return (z);
# }

# int main() {
#     a=4;
#     b=5;
#     b = soma(a, b);
#     return (b);
# }

.section .data
    A: .quad 0
    B: .quad 0
.section .text
.globl _start
soma:
    pushq %rbp
    movq %rsp, %rbp
    
    subq $8, %rsp   # aloca memoria para variavel local
    movq 16(%rbp), %rax # %rax <- b
    addq 24(%rbp), %rax # %rax <- b + a
    movq %rax, -8(%rbp)
    
    addq $8, %rsp   # desaloca memoria da variavel local
    popq %rbp
    ret
_start:
    movq $4, A  # a <- 4
    movq $5, B  # b <- 5
    pushq B
    pushq A
    call soma
    addq $16, %rsp # libera a pilha
    movq %rax, %rdi
    movq $60, %rax
    syscall

