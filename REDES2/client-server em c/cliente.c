/* Um Cliente TCP/IP 
Cliente manda uma msg (string) para o Servidor, o Servidor recebe e manda
de volta para o Cliente */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int sockdescr;
    int numbytesrecv;
    struct sockaddr_in enderec_servidor;
    struct hostent *hp; // host pointer <- saída do dns
    char buf[BUFSIZ+1];
    char *host;         // nome do host onde o servidor está executando
    char *dados;        // dados a transmitir

    if (argc != 4) {
        puts("Uso correto: cliente <nome-servidor> <porta> <dados>");
        exit(1);
    }

    host = argv[1];
    dados = argv[3];

    // chama o dns para detectar o end ip do servidor (roda o mundo todo)
    if ((hp = gethostbyname(host)) == NULL) {
        puts("Não consegui o endereço IP do servidor!");
        exit(1);
    }

    bcopy((char *)hp->h_addr, (char *)&enderec_servidor.sin_addr, hp->h_length);
    enderec_servidor.sin_family = AF_INET; // referencia TCP/IP

    // Internet   - Big Endian
    // Computador - Little Endian
    // precisa fazer essa conversão da porta nesse caso,
    // htons(host to network system)
    enderec_servidor.sin_port = htons(atoi(argv[2]));

    // AF_INET(internet), SOCK_STREAM(TCP), 0(ta usando TCP/IP)
    if ((sockdescr = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Não consegui abrir o socket!");
        exit(1);
    }

    // pedido da conexão TCP
    if ((connect(sockdescr, (struct sockaddr *)&enderec_servidor, sizeof(enderec_servidor))) < 0) {
        puts("Não consegui conectar ao servidor!");
        exit(1);
    }

    // transmissão dos dados
    if (write(sockdescr, dados, strlen(dados)) != strlen(dados)) {
        puts("Não consegui transmitir para o servidor!");
        exit(1); // se isso acontecer é pq a conexão caiu
    }

    // espera chegar a resposta do servidor e escreve a reposta
    read(sockdescr, buf, BUFSIZ);
    printf("Sou o cliente, recebi: %s\n", buf);

    close(sockdescr);
    exit(0);
}
