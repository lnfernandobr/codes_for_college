#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <sys/wait.h>

char * itoa (int value, char *result, int base)
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

const char *urlOne = "matrizA.txt";
const char *urlTwo = "matrizB.txt";
const char *urlThree = "resposta.txt";

short SocketCreate(void) {
    short hSocket;
    printf("Socket criado!\n");
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return hSocket;
}

int BindCreatedSocket(int hSocket) {
    int iRetval=-1;
    int ClientPort = 90190;
    struct sockaddr_in  remote={0};
    remote.sin_family = AF_INET; /* Internet address family */
    remote.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    remote.sin_port = htons(ClientPort); /* Local port */
    iRetval = bind(hSocket,(struct sockaddr *)&remote,sizeof(remote));
    return iRetval;
}

//try to connect with server
int SocketConnect(int hSocket)
{

    int iRetval=-1;
    int ServerPort = 90190;
    struct sockaddr_in remote={0};

    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(ServerPort);

    iRetval = connect(hSocket , (struct sockaddr *)&remote , sizeof(struct sockaddr_in));


    return iRetval;
}

//receive the data from the server
int SocketReceive(int hSocket,char* Rsp,short RvcSize)
{

    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;

    if(setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;

    }
    shortRetval = recv(hSocket, Rsp , RvcSize , 0);

    printf("Response %s\n",Rsp);

    return shortRetval;
}

// Send the data to the server and set the timeout of 20 seconds
int SocketSend(int hSocket,char* Rqst,short lenRqst)

{

    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 20;  /* 20 Secs Timeout */
    tv.tv_usec = 0;

    if(setsockopt(hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv,sizeof(tv)) < 0)
    {
        printf("Time Out\n");
        return -1;
    }
    shortRetval = send(hSocket , Rqst , lenRqst , 0);

    return shortRetval;
}

void multiply(int N, int Ma[][N], int Mb[][N], int Mc[][N], int i) {

    printf("\nO Processo %d esta calculando a linha %i\n", getpid(), i + 1);

    char SendToServer[100] = "1 1 1 1";
    char tam[100];
    char name[100];
    int aux = 0;
    char SendTO[100];
    int vetorINTEIROS[4];

    for(int j = 0; j < 4; j++) {
        for(int k = 0; k < 4;k++) {
            aux = aux + (Ma[i][k] * Mb[k][j]);
        }
        Mc[i][j] = aux;
        vetorINTEIROS[j] =Mc[i][j];
        aux = 0;
    }

    printf("Processo %d resolveu a linha %i - O resultado foi:  \n", getpid(), i + 1);
    for (int l = 0; l < 4; ++l) {
        printf("%d ", vetorINTEIROS[l]);
        SendToServer[l] = vetorINTEIROS[l];
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


        int socket_desc , sock , clientLen , read_size;
        struct sockaddr_in server , client;
        char client_message[200]={0};
        char message[100] = {0};

        //Create socket
        socket_desc = SocketCreate();
        if (socket_desc == -1) {
            printf("Could not create socket");
            return 1;
        }

        printf("Socket Criado com sucesso!\n");

        //Bind
        if( BindCreatedSocket(socket_desc) < 0) {
            //print the error message
            perror("bind failed.");
            return 1;
        }

        printf("Bind Feito! \n");


        // Ouvindo
        listen(socket_desc , 3);


        printf("Aguardando as respostas dos filhos...\n");
        for (int k = 0; k < N; ++k) {
            arrayPID[k] = fork();

            if(arrayPID[k] == 0) {
                multiply(N, Ma, Mb, Mc, k);
            } else {
                exit(0);
            }
        }


        do {
            clientLen = sizeof(struct sockaddr_in);

            // Aceita conxção do cliente
            sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&clientLen);

            if (sock < 0) {
                perror("accept failed");
                return 1;
            }

            printf("\nCliente Foi conectado - ");
            memset(client_message, '\0', sizeof client_message);
            memset(message, '\0', sizeof message);

            //Receive a reply from the client
            if( recv(sock , client_message , 200 , 0) < 0) {
                printf("recv failed");
                exit(1);
            }

            printf("Mensagem recebida da Multiplicação %d:  ", countConnection);

            for (int i = 0; i < N; ++i) {
                printf("%d ", client_message[i]);
                fprintf(fileResponse, "%d ", client_message[i]);
            }  fputc('\n', fileResponse);


            close(sock);
            fclose(fileOne);
            fclose(fileTwo);
            fclose(fileResponse);

            countConnection++;

        } while (countConnection  != N);

    } else if(pid == 0) exit(0);


    printf("\n");
    return 0;
}