/* Um Servidor TCP/IP
Aguarda a msg(string) de algum cliente, recebe uma string e manda de volta*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h> // dns
#include <stdlib.h>
#include <string.h>

#define TAMFILA 5 // tamanho da fila do servidor
#define MAXHOSTNAME 30

int main(int argc, char **argv) 
{
    int sock_escuta, sock_atende;
    int i;
    char buf[BUFSIZ+1];
    struct sockaddr_in enderec_local, enderec_cliente;
    struct hostent *hp;
    char localhost[MAXHOSTNAME];

    if (argc != 2) {
        puts("Uso correto: servidor <porta>");
        exit(1);
    }

    gethostname(localhost, MAXHOSTNAME);

    if ((hp = gethostbyname(localhost)) == NULL) {
        puts("Não consegui meu próprio endereço IP");
        exit(1);
    }

    bcopy((char *)hp->h_addr, (char *)&enderec_local.sin_addr, hp->h_length);
    enderec_local.sin_port = htons(atoi(argv[1]));
    enderec_local.sin_family = AF_INET;

    // abrir o socket q escuta
    if ((sock_escuta = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Não consegui abrir o socket!");
        exit(1);
    }

    // amarra socket a porta
    if ((bind(sock_escuta, (struct sockaddr *)&enderec_local, sizeof(enderec_local))) < 0) {
        puts("Não consegui fazer o bind!");
        exit(1);
    }

    listen(sock_escuta, TAMFILA);

    while (1) {
        i = sizeof(enderec_local);
        if ((sock_atende = accept(sock_escuta, (struct sockaddr *)&enderec_cliente, &i)) < 0) {
            puts("Não consegui estabelecer a conexão!");
            exit(1);
        }

        read(sock_atende, buf, BUFSIZ);
        printf("Sou o servidor, recebi a mensagem ---> %s\n", buf);
        write(sock_atende, buf, BUFSIZ);
        close(sock_atende);
    }
    
    exit(0);
}