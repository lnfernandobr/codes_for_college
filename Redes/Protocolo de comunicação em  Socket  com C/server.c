#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Define porta do socket
#define PORT 5000

// Tamanho do Buffer 
#define BUFFER_LENGHT 4096


int main(void) {

    // Estruturas para armazenar informações do cliente e servidor
    struct sockaddr_in client, server;

    int serverFd, clientFd;
    
    // Conteudo enviado de um socket para o outro
    char buffer[BUFFER_LENGHT];

    fprintf(stdout, "Server Online!\n");
    
    // Criando um socket IPv4
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFd == -1) {
        perror("Nao foi possivel criar o server socket\n");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Socket server criado com fd = %d\n", serverFd);

    // Definindo as propriedados do server 
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    memset(server.sin_zero, 0x0, 8);

    // Tratamento de erros com portas já em uso
    int bool = 1;
    if(setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &bool, sizeof(int)) == -1) {
        perror("Erro em opcoes de socket\n");
        return EXIT_FAILURE;
    }

    // Definir um socket a porta
    if(bind(serverFd, (struct sockaddr*) &server, sizeof(server))  == -1) {
        perror("Socket bind error\n");
        return EXIT_FAILURE;
    }

    // Começa a esperar conexções de clientes
    if(listen(serverFd, 1) == -1) {
        perror("Listen error");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Escutando na porta: %d\n", PORT);




    socklen_t client_len = sizeof(client);
    if(
        (clientFd = 
        accept(
            serverFd,
            (struct sockaddr *) &client,
            &client_len
        )) == -1) {

        perror("Nao foi aceito, erro!\n");
        return EXIT_FAILURE;
    }

    // Copia no buffer mensagem de boas vindas
    strcpy(buffer, "Hello World!\n\0");

    // Envia a mensagem ao cliente.
    if(send(clientFd, buffer, strlen(buffer), 0)) {
        fprintf(stdout, "Cliente conectado, esperando por mensagens\n");
    }

    // Loop de comunicação com o client até que seja digitado encerrar.

    do {

        // Zerando buffer
        memset(buffer, 0x0, BUFFER_LENGHT);

        int message_len;

        if((message_len = recv(clientFd, buffer, BUFFER_LENGHT,0)) > 0) {
            buffer[message_len - 1] = '\0';
            printf("Cliente está dizendo: %s\n", buffer);
        }

        if(strcmp(buffer, "encerrar") == 0) {
            send(clientFd, "encerrar", 3, 0);
        } else {
            send(clientFd, "Recebi essa bagaça!", 4, 0); 
        }

    } while(strcmp(buffer, "encerrar"));
    
    close(clientFd);
    close(serverFd);

    printf("Conecao fechada \n\n");

    return EXIT_SUCCESS;
}