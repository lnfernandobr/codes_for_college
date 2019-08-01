//
// Created by fernando on 17/04/19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

extern int errno;

char localhost[] = "localhost"; // Neste programa o cliente roda na propir maquina

int main(int argc, char** argv) {
    struct hostent *ptrh;   // Ponteiro para entrada da tabela do host
    struct protoent *ptrp;  // Ponteiro para entrada da tabela do protocolo
    struct sockaddr_in sad; // Estrutura que armazena o endereço do servidor

    int sd; // File descriptor
    int porta;  // porta de acesso
    char *host; // Ponteiro para o nome do host
    int n; // Variavel contador
    char buffer[100]; // Grava os dados recebidos do servidor

    memset((char *)&sad, 0, sizeof(sad)); // Limpa a estrutura sockaddr
    sad.sin_family = AF_INET;

    if(argc > 1)                            // Verifica se o argumento especificado é valido
        porta = atoi(argv[1]);              // Converte o argumento para binario
    else
        porta = 5200;                       // Usa porta padrão

    if(porta > 0)                           // Verificação de valores invalidos para porta
        sad.sin_port = htons((u_short)porta);
    else {
        fprintf(stderr, "Numero de porta invalida %s\n", argv[1]);
        exit(1);
    }

    if(argc > 1)
        host = argv[1];
    else
        host = localhost;

    ptrh = gethostbyname(host);

    memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

    // Mapeamento do protociçp TCP para o número de porta
    if((int *)(ptrp = getprotobyname("tcp")) == 0) {
        fprintf(stderr, "Falha ao mapear \"tcp\" para o numero de protocolo\n");
        exit(1);
    }

    sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);

    if(connect(sd, (struct sockaddr *)&sad, sizeof(sad) ) < 0) {
        fprintf(stderr, "Falha na conexao \n");
        exit(1);
    }

    n = recv(sd, buffer, sizeof(buffer), 0);

    while(n > 0) {
        buffer[n] = '\0';
        fprintf(stderr, "Cliente: %s", buffer);
        n = recv(sd, buffer, sizeof(buffer), 0);
    }

    return 0;
}