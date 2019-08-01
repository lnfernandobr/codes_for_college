#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <sys/wait.h>

const char *urlOne = "matrizA.txt";
const char *urlTwo = "matrizB.txt";
const char *urlThree = "resposta.txt";

short SocketCreate(void) {
    short hSocket;
    printf("Socket criado!\n");
    return socket(AF_INET, SOCK_STREAM, 0);
}

int BindCreatedSocket(int hSocket) {

    int ClientPort = 90190;
    struct sockaddr_in  remote = { 0 };

    remote.sin_family = AF_INET; /* Internet address family */
    remote.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    remote.sin_port = htons(ClientPort); /* Local port */

    return bind(hSocket, (struct sockaddr *)&remote, sizeof(remote));
}

int SocketConnect(int hSocket) {

    int ServerPort = 90190;
    struct sockaddr_in remote={0};

    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(ServerPort);

    return connect(hSocket , (struct sockaddr *)&remote , sizeof(struct sockaddr_in));
}

int SocketSend(int hSocket, char *Rqst, short lenRqst) {
    return send(hSocket , Rqst , lenRqst , 0);
}

void multiply(int N, int Ma[][N], int Mb[][N], int Mc[][N], int i) {

    printf("\nO Processo %d esta calculando a linha %i\n", getpid(), i + 1);

    char SendToServer[100] = "1 1 1 1";
    char tam[100];
    char name[100];
    int aux = 0;
    char SendTO[100];
    int arrayInter[4];

    for(int j = 0; j < 4; j++) {
        for(int k = 0; k < 4;k++) {
            aux += Ma[i][k] * Mb[k][j];
        }

        Mc[i][j] = aux;
        arrayInter[j] = Mc[i][j];
        aux = 0;
    }

    printf("Processo %d resolveu a linha %i - O resultado foi:  \n", getpid(), i + 1);
    for (int l = 0; l < 4; ++l) {
        printf("%d ", arrayInter[l]);
        SendToServer[l] = arrayInter[l];
    }

    printf("\n");
    sleep(1);
    printf("Preparando para envia dado...\n");

    int hSocket, read_size;
    struct sockaddr_in server;
    char server_reply[200] = {0};

    //Create socket
    hSocket = SocketCreate();
    if(hSocket == -1) {
        printf("Could not create socket\n");
        exit(1);
    }

    //Connect to remote server
    if (SocketConnect(hSocket) < 0) {
        perror("connect failed.\n");
        exit(1);
    }

    printf("Cliente %i Conectado com servidor\n", i + 1);

    //Send data to the server
    SocketSend(hSocket , SendToServer , strlen(SendToServer));

    close(hSocket);
    shutdown(hSocket,0);
    shutdown(hSocket,1);
    shutdown(hSocket,2);

    return;
}

int main(int argc , char *argv[]) {


    pid_t pid = fork();

    if(pid > 0) {
        int N, countConnection = 0, status;

        FILE *fileOne = fopen(urlOne,"r");
        FILE *fileTwo = fopen(urlTwo,"r");
        FILE *fileResponse = fopen(urlThree, "w");

        if(!fileOne|| !fileTwo || !fileResponse) {
            perror("Erro na abertura dos arquivos!\n");
            exit(1);
        }

        fscanf(fileOne,"%d",&N);

        int
                Ma[N][N],
                Mb[N][N],
                Mc[N][N];

        pid_t arrayPID[N];

        // Le o arquivo A
        for(int i = 0; i < N; ++i) {
            for(int j = 0; j < N; ++j) {
                fscanf(fileOne, "%d", &Ma[i][j]);
            }
        }

        // Le o arquivo B
        for(int i = 0; i < N; ++i) {
            for(int j = 0; j < N; ++j) {
                fscanf(fileTwo, "%d", &Mb[i][j]);
            }
        }

        fclose(fileOne);
        fclose(fileTwo);


        int socket_desc , sock , clientLen , read_size;
        struct sockaddr_in server , client;
        char client_message[200]={0};


        socket_desc = SocketCreate();  //Create socket
        if (socket_desc == -1) {
            printf("Could not create socket");
            return 1;
        } printf("Socket Criado com sucesso!\n");


        if( BindCreatedSocket(socket_desc) < 0) {  //Bind
            //print the error message
            perror("bind failed.");
            return 1;
        } printf("Bind Feito! \n");


        // Ouvindo
        listen(socket_desc , 3);

        printf("Aguardando as respostas dos filhos...\n");
        for (int k = 0; k < N; ++k) {
            arrayPID[k] = fork();
            arrayPID[k] == 0 ? multiply(N, Ma, Mb, Mc, k): exit(0);
        }


        do {
            clientLen = sizeof(struct sockaddr_in);

            // Aceita conxção do cliente
            sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&clientLen);
            if (sock < 0) {
                perror("accept failed");
                return 1;

            } printf("\nCliente Foi conectado - ");

            memset(client_message, '\0', sizeof client_message);


            //Receive a reply from the client
            if( recv(sock , client_message , 200 , 0) < 0) {
                printf("recv failed");
                exit(1);

            } printf("Mensagem recebida da Multiplicação %d:  ", countConnection);


            for (int i = 0; i < N; ++i) {
                printf("%d ", client_message[i]);
                fprintf(fileResponse, "%d ", client_message[i]);

            } fputc('\n', fileResponse);


            close(sock);
            countConnection++;

        } while (countConnection  != N);

        fclose(fileResponse);

    } else if(pid == 0) exit(0);

    printf("\n");
    return 0;
}