#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <string.h>

/*  
    #include <arpa/inet.h>
    Biblioteca necessaria para usar inet_add 
    Permite converter um endereço IP em formato longo
*/
#include <arpa/inet.h>


// SOCK_STREAM -> TCP
// SOCK_DGRAM -> UDP

#define PORT 80
#define RESPONSE_LENGHT 2000
#define ADDRESS_SERV "172.217.162.99172.217.162.99"

int main(void) {

    // Estruturas para armazenar informações do cliente e servidor
    struct sockaddr_in  server;
    int clientFileDescription;
    
    char *buffer, response[RESPONSE_LENGHT];

    // Cria a instancia do socket
    clientFileDescription = socket(AF_INET, SOCK_DGRAM, 0); /// Parametro 0 indica o IP
    if(clientFileDescription == -1) {
        perror("Nao foi possivel criar o server socket\n");
        return EXIT_FAILURE;
    }

    // Definindo as propriedados do server 
    server.sin_addr.s_addr = inet_addr("172.217.162.99"); // Endereço do google BR
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Conectar a um servidor remoto
    if(connect(clientFileDescription, (struct sockaddr*) &server, sizeof(server)) < 0)  {
        perror("Erro ao conectar\n");
        return EXIT_FAILURE;
    }      
    
    buffer = "GET / HTTP/1.1\r\n\t\n"; // Buscando a pagina principal do site.

    if(send(clientFileDescription, buffer, strlen(buffer), 0) < 0) {
        perror("Falha ao enviar!");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Dados enviados\n");
    
    // Recebendo resposta do servidor

    printf("clientFileDescription = %d", clientFileDescription);

    int len = recv(clientFileDescription, response, RESPONSE_LENGHT, 0);
    if( len < 0) {
        perror("Falha ao receber mensagem\n");
        return EXIT_FAILURE;
    }

    fprintf(stdout, "Resposta chegou");
    puts(response);

    close(clientFileDescription);


    return 0;
}