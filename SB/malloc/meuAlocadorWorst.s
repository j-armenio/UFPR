.section .data

    topoInicialHeap: .quad 0
    
    erroFinaliza: .string "Erro ao restaurar heap.\n"
    erroNumBytes: .string "Número de bytes inválido.\n"
    erroAloca: .string "Erro ao alocar bloco.\n"

    mapaHeap: .string "Mapa da heap:\n"
    cabeca: .string "################"
    livre: .string "-"
    ocup: .string "+"
    newL: .string "\n"

    aux: .quad 0

.section .text
.globl iniciaAlocador, finalizaAlocador, alocaMem, liberaMem, imprimeMapa

################################################################################

iniciaAlocador:
    movq $12, %rax                      # escolhe o tipo de syscall
    movq $0, %rdi                       # para retornar o valor atual 
    syscall
    movq %rax, topoInicialHeap
    ret

################################################################################
    
finalizaAlocador:
    movq $12, %rax                      # escolhe o tipo de syscall (brk)
    movq topoInicialHeap, %rdi          # define novo valor de brk
    syscall
    movq topoInicialHeap, %rax          
    
    cmpq $0, %rax                       # if ret syscall = 0
    jne fim_if_fin
    
    movq $1, %rax                       # write    
    movq $1, %rdi                       # stdout
    movq $erroFinaliza, %rsi            # inicio do buffer
    movq $25, %rdx                      # tam do buffer
    syscall
fim_if_fin:
    ret

################################################################################
    
alocaMem:
    pushq %rbp
    movq %rsp, %rbp

    subq $40, %rsp
    movq %rdi, -40(%rbp)                # tamanho em -40(%rbp)

    movq %rdi, %rax
    cmpq $0, %rax                       # if num_bytes <= 0
    jg fim_if_num0

    movq $1, %rax                       # write  
    movq $1, %rdi                       #stdout                                
    movq $erroNumBytes, %rsi            # inicio do buffer                      
    movq $28, %rdx                      # tam do buffer                         
    syscall 
   
    addq $40, %rsp
    movq $0, %rax
    popq %rbp
    ret 

fim_if_num0:
    movq topoInicialHeap, %rax          
    movq %rax, -8(%rbp)                 # -8(%rbp) <- posAtualHeap
    
    movq $12, %rax                      # escolhe o tipo de syscall (brk)
    movq $0, %rdi                       # para retornar o valor atual 
    syscall  
    movq %rax, -16(%rbp)                # topoAtualHeap em -16(%rbp) 
    
    movq $0, -24(%rbp)                  # blocoLivre = NULL em -24(%rbp)
    movq $0, -32(%rbp)                  # maiorTamanho = 0 em -32(%rbp)    

while_aloca:
    movq -8(%rbp), %rax    
    movq -16(%rbp), %rbx             
    cmpq -16(%rbp), %rax                # while posAtualHeap < topoAtualHeap
	jge fim_while_aloca                 # TA AQUI O ERRO
    
    movq -8(%rbp), %rbx
    movq (%rbx), %r10                   # %r10 = estado
    addq $8, %rbx
    movq (%rbx), %r11                   # %r11 = tamanho

    cmpq $0, %r10
    jne fim_if_while                    # if estado == 0
    movq -40(%rbp), %rax                
    cmpq %r11, %rax
    jge fim_if_while                     # && tamanho >= num_bytes
    
    cmpq $0, -32(%rbp)                  # if menor tamanho != 0
    je nn_tem_opt
    cmpq %r11, -32(%rbp)
    jg fim_if_while                     # if tamanho > maiorTamanho

nn_tem_opt:
    movq -8(%rbp), %rax
    movq %rax, -24(%rbp)                # blocoLivre = posAtualHeap
    movq %r11, -32(%rbp)                # menorTamanho = tamanho 

fim_if_while:
    shr $5, %r11                       # tamanho = tamanho / 32
    addq $1, %r11                       # tamanho++
    shl $5, %r11                       # tamanho * 32
    addq $16, %r11                      # tamanho + 16
    addq %r11, -8(%rbp)                 # posAtualHeap = tamanho

    jmp while_aloca

fim_while_aloca:
    cmpq $0, -24(%rbp)                  
    je else_bloco_livre                 # if blocoLivre
    
    movq -24(%rbp), %rbx
    movq $1, (%rbx)                     # marca estado como ocupado
    
    addq $16, -24(%rbp)
    movq -24(%rbp), %rax                # retorno = blocoLivre + 16
    
    addq $40, %rsp                      # desaloca variaveis locais
    popq %rbp                           # restaura %rbp
    ret    
    
else_bloco_livre: 
    movq -40(%rbp), %r11
    addq $31, %r11                    # novoTamanho = %r11 = num_bytes + 4095
    shr $5, %r11                       # %r11 / 32
    shl $5, %r11                       # %r11 * 32
    
    movq -16(%rbp), %r10                # %r10 = topoAtalHeap
    addq %r11, %r10                     # %r10 = topoAtualHeap + novoTamanho
    addq $16, %r10                      # %r10 += 16
    movq $12, %rax                      # define o tipo de syscall (brk)
    movq %r10, %rdi                     # define o novo topo da heap
    syscall
    
    cmpq $0, %rax
    jne alocou                           # if retorno de brk != 0
    
    movq $1, %rax                       # write                                 
    movq $1, %rdi                       # stdout                                
    movq $erroAloca, %rsi               # inicio do buffer                      
    movq $28, %rdx                      # tam do buffer                         
    syscall

    addq $40, %rsp                      # desaloca variaveis
    movq $0, %rax                       # retorno = NULL
    popq %rbp                           # restaura %rbp
    ret

alocou:
    movq -16(%rbp), %r10                # %r10 = novoBloco
    movq $1, (%r10)                     # estado do novoBloco = ocupado
    addq $8, %r10                       # %r10 = novoBloco + 8
    movq -40(%rbp), %rax
    movq %rax, (%r10)                   # novoBloco + 8 = num_bytes
    
    addq $8, %r10                       # %r10 = novoBloco + 8 + 8
    movq %r10, %rax                     # retorno = novoBloco + 16
    addq $40, %rsp                      # desaloca variaveis
    popq %rbp                           # restaura %rbp
    ret

################################################################################

liberaMem:
    pushq %rbp
    movq %rsp, %rbp                     
    
    cmpq $0, %rdi                       # if bloco
    jne tem_bloco           
    popq %rbp                           # restaura %rbp
    movq $1, %rax                       # retorno = 1
    ret

tem_bloco:
    movq %rdi, %r11
    subq $16, %r11                      # %r11 = bloco-16
    movq (%r11), %r10                   # %r10 = estado do bloco
    
    cmpq $0, %r10                       # if estado == 0
    jne ta_ocupado                      
    popq %rbp                           # restaura %rbp
    movq $2, %rax                       # retorno = 2
    ret

ta_ocupado:
    movq $0, (%r11)                     # marca como livre
    movq (%r11), %rax

    popq %rbp                           # restaura %rbp 
    movq $0, %rax                       # retorno = 0
    ret
    
################################################################################

imprimeMapa:
    pushq %rbp                          # salva antigo frame pointer
    movq %rsp, %rbp                     # configura novo frame pointer
    subq $32, %rsp                      # alocando variáveis
    
    movq $12, %rax
    movq $0, %rdi
    syscall                             # %rax <- topo atual da Heap
    movq %rax, -8(%rbp)                 # topoHeap = sbrk(0) em -8(%rbp)
    movq topoInicialHeap, %rax
    movq %rax, -16(%rbp)                # atual = topoInicialHeap em -16(%rbp)

    movq $1, %rax                       # write                                 
    movq $1, %rdi                       # stdout                                
    movq $mapaHeap, %rsi                # inicio do buffer                      
    movq $15, %rdx                      # tam do buffer                         
    syscall 

whileMapa:
    movq -16(%rbp), %rax                # rax = começo do bloco 
    movq -8(%rbp), %rbx
    cmpq %rax, -8(%rbp)                 # if atual < topoHeap
    jle fim_while_mapa

    movq (%rax), %rbx
    movq %rbx, -24(%rbp)              # -24(%rbp) = estado
    addq $8, %rax    
    movq (%rax), %rbx                
    movq %rbx, -32(%rbp)              # -32(%rbp) = tamanho

    movq $1, %rax                       # write                                 
    movq $1, %rdi                       # stdout                                
    movq $cabeca, %rsi                  # inicio do buffer                      
    movq $17, %rdx                      # tam do buffer                         
    syscall 

    movq $0, %rbx                       # i = 0

    movq -32(%rbp), %r12
    shr $5, %r12                  # tamanho = tamanho / 32
    addq $1, %r12                  # tamanho++
    shl $5, %r12                  # tamanho * 32 

    cmpq $0, -24(%rbp)                  # if estado == livre
    jne for_tamanho_ocup

for_tamanho_livre:
    cmpq %rbx, %r12                # if i < tamanho
    jle fim_fors

    movq $1, %rax                       # write
    movq $1, %rdi                       # stdout
    movq $livre, %rsi                   # inicio do buffer
    movq $1, %rdx                       # tam do buffer
    syscall

    addq $1, %rbx                       # i++
    jmp for_tamanho_livre

for_tamanho_ocup:
    cmpq %rbx, %r12                # if i < tamanho
    jle fim_fors
    
    movq $1, %rax                       # write                                 
    movq $1, %rdi                       # stdout                                
    movq $ocup, %rsi                    # inicio do buffer                      
    movq $1, %rdx                       # tam do buffer                         
    syscall   

    addq $1, %rbx
    jmp for_tamanho_ocup

fim_fors:
    movq -32(%rbp), %rax
    shr $5, %rax                  # tamanho = tamanho / 32
    addq $1, %rax                  # tamanho++
    shl $5, %rax                  # tamanho * 32
    addq $16, %rax                 # tamanho + 16
    addq %rax, -16(%rbp)           # atual  = tamanho                 
    
    jmp whileMapa     

fim_while_mapa:
   
    movq $1, %rax                       # write                                 
    movq $1, %rdi                       # stdout                                
    movq $newL, %rsi                    # inicio do buffer                      
    movq $3, %rdx                       # tam do buffer                         
    syscall

    addq $32, %rsp                      # desaloca variáveis
    popq %rbp                           # restaura %rbp
    ret 
