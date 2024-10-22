# Tradução do código para assembly:
#
# long int a, b;
#
# long int soma() {
#     long int x, y;
#     x=a;
#     y=b;
#     return (x+y);
# }
# int main() {
#     a=4;
#     b=5;
#     b = soma();
#     return (b);
# }

.section .data
    A: .quad 0
    B: .quad 0
.section .text
.globl _start
soma:
    pushq %rbp            # empilha (salva) %rbp
    movq %rsp, %rbp       # faz %rbp apontar para este R.A.(Registro de Ativação) %rbp <- %rsp
    subq $16, %rsp        # aloca espaço para 2 variáveis locais
    movq A, %rax          
    movq %rax, -8(%rbp)   # x <- a
    movq B, %rax          
    movq %rax, -16(%rbp)  # y <- b
    movq -8(%rbp), %rax
    movq -16(%rbp), %rbx 
    addq %rbx, %rax       # %rax <- x + y
    addq $16, %rsp        # libera variáveis locais
    popq %rbp             # desempilha (restaura) %rbp
    ret
_start:
    movq $4, A     # A <- 4
    movq $5, B     # B <- 5
    call soma
    movq %rax, B   # B <- %rax depois de soma()
    movq B, %rdi   # %rdi deve ter o valor de retorno
    movq $60, %rax
    syscall
