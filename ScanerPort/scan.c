//
// Created by fernando on 17/04/19.
//

// Libs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char** argv)
{

    struct hostent *host;
    struct sockaddr_in servidor;
    int busca, i, sock, inicio, fim;
    char nome_maquina[100];

    // Primeiramente vamosfornecer o nome damaqina que sera escaneada

    printf("Digite o nome da maquina ou seu IP: \n");
    fgets(nome_maquina, 100, stdin);

    // Porta de incio de escaneamento

    printf("Digite o numero inicial da porta: \n");
    scanf("%d", &inicio);

    // Porta final de escaneamento
    printf("Digite o numero final da porta: \n");
    scanf("%d", &fim);


    // Inicializando a estrutura sockaddr_in
    strncpy((char *)&servidor, "", sizeof(servidor));

    servidor.sin_family = AF_INET;

    // Usar endereço IP direto
    if(isdigit(nome_maquina[0])) {
        printf("Executando o inet_addr...\n");
        servidor.sin_addr.s_addr = inet_addr(nome_maquina);
    }

    else if( (host = gethostbyname(nome_maquina)) != 0) {
        printf("Executando gethostbyname\n");
        strncpy((char *)&servidor.sin_addr, (char *)host->h_addr, sizeof(servidor.sin_addr));
        printf("Executado!");
    }

    else {
        herror(nome_maquina);
        exit(2);
    }

    // Começa a escanear as portas
    printf("Inicio do escanemanto de portas: \n");
    for(int i = inicio; i <= fim; ++i) {
        // Preencher o numero da porta
        servidor.sin_port = htons(i);

        // Criar uma conxeção socket do tipo IP
        sock = socket(AF_INET, SOCK_STREAM, 0);

        // Conectar usando o soket e a estrutura sockaddr_in
        busca = connect(sock, (struct sockaddr*)&servidor, sizeof(servidor));

        if(busca < 0) {
            printf("Falha na funcao connect, Porta %d Fechada", i);
            fflush(stdout);
        } else {
            printf("%-5d aberta\n", i);
        }

        close(sock);

    }
    printf("\r");
    fflush(stdout);
    return 0;
}