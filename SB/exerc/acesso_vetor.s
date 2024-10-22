# Sugira uma forma de alocar, acessar e liberar variáveis que são vetores. 

# long int proc (long int n) {
#     long int i;
#     long int vetor[n];
#     ...
#     vetor[0] = 1;
#     i=n-1;
#     vetor[i] = 1;
#     vetor[i] = vetor[i-1];
# }

proc:
    pushq %rbp
    movq %rsp, %rbp

    movq 16(%rbp), %rax        # n <- rax
    salq $3, %rax              # n * 8 = tamanho do vetor
    subq %rax, %rsp            # aloca na pilha espaço suficiente para o vetor
    movq $1, (%rsp)            # vetor[0] = 1
    movq 16(%rbp), %rdi        # i <- n
    subq $1, %rdi              # i <- i-1
    movq $1, (%rsp,%rdi,8)     # desloca 8*rdi com base em rsp(i) <- 1
    movq -8(%rsp,%rdi,8), %rax # rax <- desloca -8*rdi com base em rsp(i)
    movq %rax, (%rsp,%rdi,8)   # desloca 8*rdi com base em rsp(i) <- rax
    movq %rbp, %rsp            # libera espaço do vetor

    popq %rbp
    ret
