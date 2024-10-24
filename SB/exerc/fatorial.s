void fat(long int *res, long int n) {
    long int r;
    if (n<=1)
        *res = 1; // joga 1 na variavel cujo endereço esta em res
    else {
        fat(res, n-1);
        r = *res * n;
        *res = r;
    }
}

int main(long int argc, char **argv) {
    long int x;
    fat(&x, 3);
    return (x);
}

.section .data
.section .text
.globl _start
fat:

_start:
    pushq %rbp
    movq %rsp, %rbp


