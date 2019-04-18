// Servidor simples em c


// Libs
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

// Variaveis globais
int visitas = 0;


int main(int argc, char** argv) {
    struct hostent *ptrh;   // Ponteiro para entrada da tabela do host 
    struct protoent *ptrp;  // Ponteiro para entrada da tabela do protocolo
    struct sockaddr_in sad; // Estrutura que armazena o endereço do servidor
    struct sockaddr_in cad; // EStrutura que armazena o endereço dos clientes

    int sd, sd2;     // Descritor de socket
    int porta;       // Numero da porta do protocolo
    int alen;        // Tamanho do endereço
    char buf[1000];  // BUffer para armazenar as mensagens enviadas pelo servidor

    memset( (char *)&cad, 0, sizeof(sad)); // Limpa a estrutura sockaddr

    sad.sin_family = AF_INET;               // Configura a familia de protocolo de internet
    sad.sin_addr.s_addr = INADDR_ANY;       // Libera para qualquer cliente acessar


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
        
    // Mapeamento do protociçp TCP para o número de porta
    if( (int *)(ptrp = getprotobyname("tcp")) == 0) {
        fprintf(stderr, "Falha ao mapear \"tcp\" para o numero de protocolo\n");
        exit(1);
    }

    // Cria o socket                  ptrp->p_proto == endereço do protocolo 
    sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);

    if( bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0 ) {
        fprintf(stderr, "Falha no Bind\n");
        exit(1);
    }

    // Listen
    if(listen(sd, 5) < 0) {
        fprintf(stderr, "Falha na funcao listen \n");
        exit(1);
    }


    // Loop do servidor - aceite as conexoes e mantenha os pedidos de conexao
    fprintf(stderr, "Servidor ativo e funcionando\n");

    while(1) {
        alen = sizeof(cad);
        fprintf(stderr, "SERVIDOR: Aguardando conexoes...\n");
        
        if( (sd2 = accept(sd, (struct sockaddr *)&cad, &alen)) < 0 ) {
            fprintf(stderr, "Falha na funcao accept\n");
            exit(1);
        }
        visitas++;
        // sprinf(buf, "Este servidor foi contato %d vezes %s\n", visitas == 1 ? "." : "s.");
        fprintf(stderr, "SERVIDOR: %s", buf);
        
        send(sd2, buf, strlen(buf), 0);

        close(sd2);
    }

}