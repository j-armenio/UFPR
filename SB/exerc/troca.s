# long int a, b;
# void troca(long int *x, long int *y) {
#     long int z;
#     z = *x;
#     *x = *y;
#     *y = z;
# }
# int main() {
#     a=1;
#     b=2;
#     troca(&a, &b);
#     return(0);
# }

.section .data
    A: .quad 0
    B: .quad 0
.section .text
.globl _start
troca:
    pushq %rbp
    movq %rsp, %rbp

    subq $8, %rsp        # long int z (em -8(%rbp))

    movq 16(%rbp), %rax # rax := x
    movq (%rax), %rbx   # end. apont. por %rax (*x)
    movq %rbx, -8(%rbp) # z := *x

    movq 24(%rbp), %rax # %rax := y
    movq (%rax), %rbx   # %rbx := end. apont. por %rax (*y)
    movq 16(%rbp), %rax # %rax := x
    movq %rbx, (%rax)   # *x = *y

    movq -8(%rbp), %rbx # %rbx := z
    movq 24(%rbp), %rax # %rax := y
    movq %rbx, (%rax)   # *y = z 
    
    addq $8, %rsp
    popq %rbp
    ret
_start:
    movq $1, A
    movq $2, B

    pushq $B # empilha end. B
    pushq $A # empilha end. A
    call troca
    addq $16, %rsp # libera espaço dos parametros

    movq B, %rdi
    movq $60, %rax
    syscall
