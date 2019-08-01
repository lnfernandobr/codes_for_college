#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <poll.h>

#define TIMEOUT 100
 
typedef struct sockaddr_in sock_In ;
typedef struct sockaddr sock_addr ;
typedef struct pacote Pacote;
typedef struct frame Frame;


const uint32_t MOD_ADLER = 65521;

struct pacote {
    char string[2000];
};
struct frame {
    int ack;
    int tipo;
    int porta;
    int proxSeq;
    uint32_t checksum;
    Pacote pacote;
};


void escutaResposta(int sockfd, Frame *frame_enviar, sock_In *servidor, struct pollfd *poll_set);
void enviaPacote(int socket, Frame *frame_enviar, sock_In *servidor);
short criaSocket(sock_In *servidor, int port, char *endereco);
int main(int argc, char **argv);
void pegaMensagem(char *buffer);



/* string é a localização dos dados na memória física e len é o tamanho dos dados em bytes */
uint32_t checksum_verifica(char *data, size_t len) {
    uint32_t a = 1, b = 0;
    size_t index;

    // Processar cada byte dos dados em ordem
    for (index = 0; index < len; ++index) {
        a = (a + data[index]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}




int handshaking(int socket, sock_In *servidor) {

    struct pollfd poll_set[1];
    poll_set[0].fd = socket;
    poll_set[0].events = 0;
    poll_set[0].events |= POLLIN;

    // Seta pacote
    Frame frame_enviar;
    frame_enviar.tipo = 0;
    frame_enviar.proxSeq = 1;
    strcpy(frame_enviar.pacote.string, "Olá");

    Frame n_frame_recebido;
    socklen_t tamanho_servidor = sizeof(*servidor);

    printf("####### Enviando pedido de conexão... #######\n");
    sendto(socket, &(frame_enviar), sizeof(Frame), 0, (sock_addr*)&(*servidor), sizeof(*servidor));

    int tamanho_recebido = recvfrom(socket, &n_frame_recebido, sizeof(Frame), 0, (sock_addr *) &(*servidor), &tamanho_servidor);

    if(tamanho_recebido > 0) {
        printf("Nova porta de conexão: %d\n", n_frame_recebido.porta);

        int port = htons(n_frame_recebido.porta);
        printf("Conexão estabelecida com sucesso\n");
        printf("\n\n\n");
        return port;
    }

    return 0;
}



/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função reenvia o pacote em um intervalo de tempo TIMEOUT
 * caso nenhum resposta do servidor seja recebida, ele tenta
 * por 5 tentativas, apos essas tentativas, o programa é finalizado
 * o frame recebido ao servidor
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void escutaResposta(int sockfd, Frame *frame_enviar, sock_In *servidor, struct pollfd *poll_set) {

    uint32_t resendQuadro  = 0;

    while (poll(poll_set, 1, TIMEOUT) == 0) {
        printf("Reenviando pacote numero: %i \n", resendQuadro + 1);
        enviaPacote(sockfd, frame_enviar, servidor);
        resendQuadro++;

        if(resendQuadro == 5) {
            printf(
                "A Mensagem foi enviada ao servidor várias vezes..."
                "Nenhuma resposta do mesmo foi obtida. Tenha certeza que o "
                "servidor esta online e respondendo conexões desse cliente.\n"
            );

            printf("\n\nO Cliente sera finalizado... Bye\n");
            exit(1);
        }
    }
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: Função envia o frame recebido ao servidor           ::*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void enviaPacote(int socket, Frame *frame_enviar, sock_In *servidor) {
    sendto(socket, &(*frame_enviar), sizeof(Frame), 0, (sock_addr*)&(*servidor), sizeof(*servidor));
    printf("[+] Mensagem enviada\n");
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: Função pega mensagem do usuario                     ::*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void pegaMensagem(char *buffer) {
    printf("String a ser enviada ao servidor [UDP]\n");
    char message[100];
    fgets(message, 100, stdin);

    strcpy(buffer, message);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: Função retorna um socket, ou exit com falha de erro ::*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
short criaSocket(sock_In *servidor, int port, char *endereco) {

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Falha na criação do socket\n");
        exit(-3);
    }

    memset(&(*servidor), '\0', sizeof(*servidor));
    if(strcmp(endereco, "localhost") == 0) {
        strcpy(endereco, "127.0.0.1");
    }  

    

    servidor->sin_family = AF_INET;
    servidor->sin_port = htons(port);
    servidor->sin_addr.s_addr = inet_addr(endereco);


    return sock;
}




int main(int argc, char **argv) {

    sock_In  servidor;
    Frame frame_recebido;
    Frame frame_enviar;

    int tamanho_recebido, sockfd;

    if(argc != 3) {
        printf("Formato %s <porta> <endereço> \n", argv[0]);
        exit(0);
    }

    sockfd = criaSocket(&servidor, atoi(argv[1]), argv[2]); // Cria socket

    servidor.sin_port = handshaking(sockfd, &servidor); // Estabelece conexão com servidor

    printf(":::::::::::::::::: Conexão estabelecida com sucesso ::::::::::::::::::\n");

    socklen_t tamanho_servidor = sizeof(servidor);

    /// Struct para timeout
    struct pollfd poll_set[1];
    poll_set[0].fd = sockfd;
    poll_set[0].events = 0;
    poll_set[0].events |= POLLIN;

    int proxSeq = 0; // Faz o controle de qual pacote enviar e qual ack receber

    // Seta frame_recebido para primeira conexão
    frame_recebido.ack = 0;
    frame_enviar.porta = -12;
    int numeroReenvio = 0;

    while (1) {
        if(proxSeq == frame_recebido.ack) { // Se o ack é o correto, envia o proximo pacote

            frame_enviar.tipo = proxSeq;
            frame_enviar.proxSeq = !proxSeq;
            frame_enviar.porta = 0;
            pegaMensagem(frame_enviar.pacote.string);

            int  len =  checksum_verifica(frame_enviar.pacote.string, strlen(frame_enviar.pacote.string)); // Faz a soma dos bytes
            frame_enviar.checksum = len;

            proxSeq = !proxSeq;

            sendto(
                sockfd,
                &frame_enviar,
                sizeof(Frame),
                0,
                (sock_addr*)&servidor,
                sizeof(servidor)
            );
            
            if(strcmp(frame_enviar.pacote.string, "sair\n") == 0) {
                close(sockfd);
                printf("Fechando Aplicação do cliente\n");
                exit(0);
            }
        
        } else {
            sendto(
                sockfd,
                &frame_enviar,
                sizeof(Frame),
                0,
                (sock_addr*)&servidor,
                sizeof(servidor)
            );
        }

        escutaResposta(sockfd, &frame_enviar, &servidor, poll_set); // Nesse ponto se o TimeOut estorar, reenvia o pacote,
        tamanho_recebido = recvfrom(sockfd, &frame_recebido, sizeof(Frame), 0, (sock_addr*) &servidor, &tamanho_servidor);

        if(tamanho_recebido > 0) {


            if(frame_recebido.ack == proxSeq) {
                numeroReenvio = 0;

                printf("\n[ACK %d] Recebido \n", frame_recebido.ack);

                if(strcmp(frame_recebido.pacote.string, "Dados Salvos!") == 0)
                    printf("Seus dados foram salvos com sucesso no banco de dados :)\n");

                else {
                    printf("::::::::::::::::: >> Resultado da sua busca: \n\n");

                    if(strcmp(frame_recebido.pacote.string, "NOT") == 0) {
                        printf("Ops, parece que não temos nenhum posto de combustivel perto de você :( \n");
                        printf("Tente aumentar o raio de busca ou alterar o tipo de combustivel\n\n");
                    } else
                        printf("\"------------> %s\n",  frame_recebido.pacote.string);

                }
            }

            else {
                // Se o ACK recebido não é o esperado, Reenvia o pacote
                printf("Reenviando Pacote\n");
                sendto(
                    sockfd,
                    &frame_enviar,
                    sizeof(Frame),
                    0,
                    (sock_addr*)&servidor,
                    sizeof(servidor)
                );
                numeroReenvio++;
                if(numeroReenvio == 5) exit(0);

            }
        }

    }
}

