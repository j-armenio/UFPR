.section .data
a:      .quad 0   
b:      .quad 0   

.section .text
.globl _start
_start:
    movq $4, %rdi       
    movq %rdi, a(%rip)  

    movq $5, %rax       
    movq %rax, b(%rip)  

    
    movq a(%rip), %rdi  
    movq b(%rip), %rax  
    cmpq %rax, %rdi     
    jle else            

    addq %rax, %rdi     
    movq %rdi, a(%rip)  
    jmp fim_if          

else:
    subq %rax, %rdi     
    movq %rdi, a(%rip) 

fim_if:
    movq a(%rip), %rdi  

    movq $60, %rax      
    syscall             
