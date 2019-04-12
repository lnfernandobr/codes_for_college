/* A Função bind associa o socket a uma porta a um endereço particular */ 

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> // Para usar a função write

int main(void) {
    
    int meu_socket, novo_socket, c;
    struct  sockaddr_in servidor, cliente;

    char *mensagem;
    
    meu_socket = socket(AF_INET, SOCK_STREAM, 0);

    servidor.sin_family =  AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY; // Qualquer endereço de entrada
    servidor.sin_port = htons(8881);
    
    if( bind(meu_socket, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
        puts("Erro no bind!\n");
    } else {
        puts("Executado com sucesso!\n");        
    }

    
    // Listen, serve par aiuvir as conexções, 3 -> numero de conexção que maquinas poderam se conectar 
    // ao servidor
    listen(meu_socket, 3);
    puts("Aguardando novas conecções\n");
    c = sizeof(struct sockaddr_in);
    
    
    
    while( (novo_socket = accept(meu_socket, (struct sockaddr *)&cliente, (socklen_t *)&c )) ) {
       
        if(novo_socket < 0) {
            puts("Erro ao aceitarq a conexção");
        } else {
            puts("\n Conexção aceita!\n");
        }
        
        puts("\n");

        // Respondendo ao cliente

        mensagem = "Que droga mano whi!, flws\n";

        write(novo_socket, mensagem, strlen(mensagem));
    }
    

    return 0;
}