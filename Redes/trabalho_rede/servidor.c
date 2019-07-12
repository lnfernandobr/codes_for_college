#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <sys/wait.h>
#include <poll.h>
#include <ctype.h>

#pragma GCC diagnostic ignored "-Wcomment"

#define PI 3.14159265358979323846


typedef struct sockaddr_in sock_In ;
typedef struct sockaddr sock_addr ;
typedef struct frame Frame;
typedef struct pacote Pacote;
typedef struct valorRecebidoPesquisa valores_pesquisa;



unsigned int true = 1;


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
struct valorRecebidoPesquisa {
    int raio;
    unsigned int tipoCombustivel;
    double latitude_origem;
    double longitude_origem;
};
struct distancia_posto {
    double distancia;
    int preco;
    double latitude;
    double longitude;
};


const char *COMBUSTIVEL_DIESEL = "combustivel_diesel.txt";
const char *COMBUSTIVEL_ALCOOL = "combustivel_alcool.txt";
const char *COMBUSTIVEL_GASOLINA = "combustivel_gasolina.txt";
const uint32_t MOD_ADLER = 65521;


struct distancia_posto obterMenorPrecoAutoPosto(int num_linhas_arquivo, FILE *arquivo, valores_pesquisa valores_do_pacote_pesquisa);
void enviaResposta(Frame *frame_recebido, int32_t socket, sock_In servidor, const char *mensagem, int *proxSeq, Frame *frame_enviar);
void trata_solicitacao(Frame *frame_recebido, int socketServidor, sock_In cliente, int *proximaSeq, Frame *frame_enviar);
double distanciaDoisPontos_Haversine(double lat1, double lon1, double lat2, double lon2);
void pesquisa_dados(Frame frame_recebido, char *mensagem, int len);

valores_pesquisa normalizaPacoteDePesquisa(Frame frame_recebido);
void escreveArquivo(const char *mensagem, const char *url);
int gera_e_envia_porta(int serverSocket, sock_In cliente);
void bindSock(sock_In *servidor, int32_t sock, int porta);
void enviarPorta(int porta, int Socket, sock_In cliente);
unsigned int trateEntrada(int argc, char **argv);
void inicia_cliente(sock_In *cliente, int port);
FILE *obterRefArquivo(int tipoCombustivel);
unsigned int contarLinhaArquivo(int tipo);
void imprimePacote(Frame *frame_recebido);
void salvar_dados(Frame frame_recebido);
FILE *lerArquivo(const char *url);
int main(int argc, char **argv);
double deg2rad(double deg);
double rad2deg(double rad);
short criaSock();




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


void normalizaPacoteDeDados(Frame frame_recebido, int *tipoCombustivel, char *mensagem, int len) {

    double valores[3], num;
    char *delimitador = " ";
    char *ptr = strtok(frame_recebido.pacote.string, delimitador);

    int i = 0, count = 0;

    while(ptr != NULL) {
        if(i >= 2) {
            sscanf(ptr,"%lf",&num);
            valores[count] = num;
            count++;
        }

        i++;
        ptr = strtok(NULL, delimitador);
    }

    *tipoCombustivel = frame_recebido.pacote.string[2] - '0';


    snprintf (
        mensagem,
        len,
        "%d %lf %lf %c",
        (int) valores[0],
        valores[1],
        valores[2],
        '\0'
    );

}









/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Faz pré tratamento na string recebida
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int trata_mensagem(char *string) {

    if(strlen(string) == 0)
        return -1;



    if(string[0] != 'D' && string[0] != 'd' && string[0] != 'P' && string[0] != 'p')
        return -1;



    if(string[0] == 'd' || string[0] == 'D') {

        if(string[1] != ' ')
            return -1;

        if(string[2] != '0' && string[2] != '1' && string[2] != '2')
            return -1;

        if(string[3] != ' ')
            return -1;


        return 1;
    }

    if(string[0] == 'p' || string[0] == 'P') {
        if(string[1] != ' ')
            return -1;

        if(string[2] != '0' && string[2] != '1' && string[2] != '2') return -1;

        if(string[3] != ' ')
            return -1;


        return 0;
    }

    return 0;
}




/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * retorna porta aleatoria gerada pelo S.0
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int gera_porta_aleatoria(struct sockaddr_in cliente) {

    int porta, Socket = socket(AF_INET, SOCK_DGRAM, 17);

    sock_In auxiliar;
    socklen_t len = sizeof(auxiliar);

    cliente.sin_family = AF_INET;
    cliente.sin_port = htons(0);
    cliente.sin_addr.s_addr = INADDR_ANY;


    if(bind(Socket,(struct sockaddr *)&cliente, sizeof(cliente)) < 0){
        perror("Bind");
        close(Socket);
        exit(-4);
    }

    // copia os dados para o velho_cliente
    getsockname(Socket,(sock_addr *)&auxiliar, &len);

    // ntohs() converte de network byte order para host byte order
    porta = ntohs(auxiliar.sin_port);
    printf("Porta gerada: >> %d\n", porta);
    close(Socket);

    return porta;
}




/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Chama função para gerar porta, pega a porta gerada, e chama
 * outra função para enviar e retorna valor da porta
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

int gera_e_envia_porta(int serverSocket, sock_In cliente) {
    puts("S.O esta gerando uma nova porta!!");
    int porta = gera_porta_aleatoria(cliente);

    puts("\n######### Enviando nova porta para cliente solicitante #########\n");
    enviarPorta(porta, serverSocket, cliente);

    return porta;
}




/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Função envia porta para o cliente                             :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void enviarPorta(int porta, int Socket, sock_In cliente) {

    socklen_t len = sizeof(cliente);
    Frame frame_enviar;
    frame_enviar.porta = porta;

    if(sendto(Socket, &frame_enviar, sizeof(Pacote), 0, (sock_addr *)&cliente, len) < 0) {
        perror("Send porta");
        close(Socket);
        exit(-5);
    }
}




/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função [enviaResposta] envia o pacote de resposta para o cliente. a função recebe o socket ao
 * qual deve enviar o frame, a struct frame_recebido serve para saber informações de controle de
 * envio de ack e para controle de proximo quadro que deve receber. a mensagem é variavel que de fato
 * sera enviada para o cliente e que é util a ele. a variavel proxSeq serve para chegar se o quadro
 * recebido esta de acordo com o que era esperado, o frame enviar passado por referencia é para que ele
 * seja setado na propria função, e caso o proximo pacote seja duplicado, uma função de nivel superior faz
 * esse tratamento, assim só reenviando a struct setada na função enviaResposta.
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void enviaResposta(Frame *frame_recebido, int32_t socket, sock_In servidor, const char *mensagem, int *proxSeq, Frame *frame_enviar) {

    printf("Pacote recebido: %d\n", frame_recebido->tipo);
    printf("Pacote que devo receber: %d\n", *proxSeq);

    if(frame_recebido->tipo == *proxSeq) {
        frame_enviar->ack = frame_recebido->proxSeq;
        *proxSeq = frame_recebido->proxSeq;

        strcpy(frame_enviar->pacote.string, mensagem);
        sendto(
            socket,
            (const char *)&(*frame_enviar),
            sizeof(Frame),
            MSG_CONFIRM,
            (sock_addr*)&servidor,
            sizeof(servidor)
        );

        printf(":::::::::::::: [ACK %d] Enviado com sucesso.\n", *proxSeq);
        printf("\n\n\n");

    }
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função faz o bind na estrutura servidor com o socket e a porta
 * passados, faz um bind para receber conexões de todos os endereços
 * (INADDR_ANY), retorna erro -4 caso não consiga fazer o bind
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void bindSock(sock_In *servidor, int sock, int porta) {

    memset(&(*servidor), 0, sizeof(*servidor));

    servidor->sin_family = AF_INET;
    servidor->sin_port = htons(porta);
    servidor->sin_addr.s_addr = INADDR_ANY;


    int booleanoBind = bind(sock, (sock_addr*)&(*servidor), sizeof(*servidor));

    if (booleanoBind < 0) {
        perror("Bind falhou \n");
        exit(-4);
    }

}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Converte decimal para Radianos                                :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg) {
    return (deg * PI / 180);
}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*::  Converte Radianos para decimal                                :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad) {
    return (rad * 180 / PI);
}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/*  Função retorna distancia entre dois pontos dados por coordenadas num globo
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double distanciaDoisPontos_Haversine(double lat1, double lon1, double lat2, double lon2) {

    double teta, dist;

    if ((lat1 == lat2) && (lon1 == lon2))
        return 0;

    teta = lon1 - lon2;
    dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) * cos(deg2rad(lat2)) * cos(deg2rad(teta));
    dist = acos(dist);
    dist = rad2deg(dist);
    dist = dist * 60 * 1.1515;

    return dist *= 1.609344;
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/* Função faz a contagem de linhas de um arquivo, retorna 0 caso o arquivo não possa ser aberto  ::
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
unsigned int contarLinhaArquivo(int tipo) {
    FILE *arquivo = NULL;

    arquivo = obterRefArquivo(tipo);
    if(arquivo == NULL)
        return 0;

    char c, letra = '\n';
    unsigned int vezes = 0;

    while(fread (&c, sizeof(char), 1, arquivo)) {
        if(c == letra) {
            vezes++;
        }
    }

    fclose(arquivo);

    return vezes;
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/*:: Função retorna um socket, ou exit com falha de erro ::
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
short criaSock() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        perror("Falha na criação do socket\n");
        exit(-4);
    }

    return sock;

}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * função retorna uma struct contendo informações como distancia, latitude, longitude, e preco do posto de combustivel
 * mais mais barato proximo ao raio de busca em que o usuario passou.
 *
 * A Função usa dois vetores de estruturas, um vetor A e um vetor B, no vetor A, coloca todos os postos de
 * combustiveis ao qual o tipo de combustivel o usuario escolheu, depois faz um filtro colocando no vetor B, todos arqueles que
 * estão no vetor A e são menores ou iguais ao raio de busca.
 *
 * Apos esse filtro, é feito um segundo, colocando na estrutura resposta, o posto de combustivel mais barato dos quais estão
 * perto de você, apos isso a estrutura é retornada.
 *
 * Na função tem varios logs das tabelas
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
struct distancia_posto obterMenorPrecoAutoPosto(int num_linhas_arquivo, FILE *arquivo, valores_pesquisa valores_do_pacote_pesquisa) {


    int RAIO = valores_do_pacote_pesquisa.raio;
    double LATITUDE = valores_do_pacote_pesquisa.latitude_origem;
    double LONGITUDE = valores_do_pacote_pesquisa.longitude_origem;
    int TIPO_COMBUSTIVEL = valores_do_pacote_pesquisa.tipoCombustivel;

    double longitudeAux, latitudeAux, distancia = 0;
    int  precoAux, i;
    struct distancia_posto resposta;
    struct distancia_posto postos_de_cumbustiveis[num_linhas_arquivo];
    struct distancia_posto vetor_auxiliar_postos_dentro_do_raio[num_linhas_arquivo];

    // Seta o vetor
    for (i = 0; i < num_linhas_arquivo; ++i) {
        vetor_auxiliar_postos_dentro_do_raio[i].distancia = -1;
        vetor_auxiliar_postos_dentro_do_raio[i].preco = -1;
        vetor_auxiliar_postos_dentro_do_raio[i].latitude = -1;
        vetor_auxiliar_postos_dentro_do_raio[i].longitude= -1;
    }

    i = 0; // Seta no vetor de estruturas postos_de_cumbustiveis, todos os postos de combustiveis
    while((fscanf(arquivo,"%d %lf %lf\n", &precoAux, &latitudeAux, &longitudeAux)) != EOF ) {
        postos_de_cumbustiveis[i].latitude = latitudeAux;
        postos_de_cumbustiveis[i].longitude = longitudeAux;
        postos_de_cumbustiveis[i].preco = precoAux;
        postos_de_cumbustiveis[i].distancia = 0; // Só seta, mas não será usado
        i++;
    }

    fclose(arquivo); // Fecha o arquivo


    printf("\n\n\nTabela de valores de todos os postos de tipo %d\n", TIPO_COMBUSTIVEL);
    for (i = 0; i < num_linhas_arquivo; ++i)
        printf(
            "Latitude: %lf, Longitude %lf, Preço: %lf\n",
            postos_de_cumbustiveis[i].latitude,
            postos_de_cumbustiveis[i].longitude,
            (float) postos_de_cumbustiveis[i].preco / 1000
        );


    i = 0;
    for (int j = 0; j < num_linhas_arquivo; ++j) {

        distancia = distanciaDoisPontos_Haversine(
                        LATITUDE,
                        LONGITUDE,
                        postos_de_cumbustiveis[j].latitude,
                        postos_de_cumbustiveis[j].longitude
                    );

        if(distancia <= RAIO) {
            vetor_auxiliar_postos_dentro_do_raio[i].distancia = distancia;
            vetor_auxiliar_postos_dentro_do_raio[i].preco = postos_de_cumbustiveis[j].preco;
            vetor_auxiliar_postos_dentro_do_raio[i].latitude = postos_de_cumbustiveis[j].latitude;
            vetor_auxiliar_postos_dentro_do_raio[i].longitude = postos_de_cumbustiveis[j].longitude;

            i++;
        }
    }


    if(vetor_auxiliar_postos_dentro_do_raio[0].distancia == -1) {
        resposta.preco = -1;
        resposta.distancia =-1;
        resposta.latitude = -1;
        resposta.longitude = -1;
        printf("\nNão Há postos de combustiveis perto da sua região\n\n\n");

        return resposta;
    }

    printf("\n\n\nTabela de valores de todos os postos de tipo %d e dentro do raio de busca: %d\n", TIPO_COMBUSTIVEL, RAIO);
    for (int k = 0; k < i; ++k) {
        printf(
            "Latitude: %lf, Longitude %lf, Preço: %d\n",
            vetor_auxiliar_postos_dentro_do_raio[k].latitude,
            vetor_auxiliar_postos_dentro_do_raio[k].longitude,
            vetor_auxiliar_postos_dentro_do_raio[k].preco
        );
    }


    printf("\n\n\n");
    resposta.preco = vetor_auxiliar_postos_dentro_do_raio[0].preco;
    resposta.distancia = vetor_auxiliar_postos_dentro_do_raio[0].distancia;
    resposta.latitude = vetor_auxiliar_postos_dentro_do_raio[0].latitude;
    resposta.longitude = vetor_auxiliar_postos_dentro_do_raio[0].longitude;


    for(int j = 0; j < i; j++) {
        if(vetor_auxiliar_postos_dentro_do_raio[j].preco < resposta.preco) {
            resposta.preco = vetor_auxiliar_postos_dentro_do_raio[j].preco;
            resposta.distancia = vetor_auxiliar_postos_dentro_do_raio[j].distancia;
            resposta.latitude = vetor_auxiliar_postos_dentro_do_raio[j].latitude;
            resposta.longitude = vetor_auxiliar_postos_dentro_do_raio[j].longitude;
        }
    }

    return resposta;
}



/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
/*:: Função retorna struct contendo os valores retirados da string, funciona como um slipt  ::
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
valores_pesquisa normalizaPacoteDePesquisa(Frame frame_recebido) {

    valores_pesquisa valoresDoPacote;

    double valores[4], num;
    char *delimitador = " ";
    char *ptr = strtok(frame_recebido.pacote.string, delimitador);

    int i = 0, count = 0;

    while(ptr != NULL) {
        if(i >= 1) {
            sscanf(ptr,"%lf",&num);
            valores[count] = num;
            count++;
        }

        i++;
        ptr = strtok(NULL, delimitador);
    }

    valoresDoPacote.tipoCombustivel = (int) valores[0];
    valoresDoPacote.raio = (int) valores[1];

    valoresDoPacote.latitude_origem = valores[2];
    valoresDoPacote.longitude_origem = valores[3];

    return valoresDoPacote;
}



/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função abre o  arquivo  solicitado em modo leitura pela url
 * passada, e retorna ponteiro. Retorna null caso não seja encontrado arquivo
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
FILE *lerArquivo(const char *url) {
    FILE *arquivo = NULL;
    arquivo = fopen(url, "r");

    if(arquivo == NULL) {
        perror("Erro, nao foi possivel abrir o arquivo\n");
        return NULL;
    }

    return arquivo;
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função retorna ponteiro que aponta para arquivo, o ponteiro pode apontar para tres tipos de arquivos diferentes,
 * file_alcool, file_gasolina, file_diesel. Caso o arquivo não seja encontrado, é retornado NULL
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
FILE *obterRefArquivo(int tipoCombustivel) {

    if(tipoCombustivel == 0)
        return lerArquivo(COMBUSTIVEL_DIESEL);

    if(tipoCombustivel == 1)
        return lerArquivo(COMBUSTIVEL_ALCOOL);

    if(tipoCombustivel == 2)
        return lerArquivo(COMBUSTIVEL_GASOLINA);

    return NULL;
}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Pesquisa dados recebe o frame recebido, com valores da requisicao do cliente, e uma variavel mensagem por parâmetro,
 * essa variavel sera alterada, e sera enviada ao cliente solicitante na funcao send_ack.
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void pesquisa_dados(Frame frame_recebido, char *mensagem, int len) {
    FILE *arquivo = NULL;
    int numeroLinhas;
    struct distancia_posto resposta;

    // Verifica se todas as validações para a pesquisa estão corretas antes de continuar
    valores_pesquisa valores_do_pacote_pesquisa = normalizaPacoteDePesquisa(frame_recebido);

    printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("\t Pacote de pesquisa: \n");
    printf("\t\t Raio: %d\n", valores_do_pacote_pesquisa.raio);
    printf("\t\t Tipo de combustivel: %d\n", valores_do_pacote_pesquisa.tipoCombustivel);
    printf("\t\t Latitude origem: %lf\n", valores_do_pacote_pesquisa.latitude_origem);
    printf("\t\t Longitude origem: %lf\n", valores_do_pacote_pesquisa.longitude_origem);
    printf("+++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");


    arquivo = obterRefArquivo(valores_do_pacote_pesquisa.tipoCombustivel);
    if(arquivo == NULL) {
        perror("Arquivo não foi aberto com sucesso, talvez a variavel [TIPO] passada a função não seja um tipo valido");
        return;
    }

    // Recebe o numero de linhas que tem no arquivo
    numeroLinhas = contarLinhaArquivo(valores_do_pacote_pesquisa.tipoCombustivel);
    if(numeroLinhas == 0)
        return;

    resposta = obterMenorPrecoAutoPosto(numeroLinhas, arquivo, valores_do_pacote_pesquisa);

    /// Se campo distancia da struct resposta retornar com -1, significa que não há postos de combustiveis perto, então
    /// então escrevemos na string mensagem [NOT], not é uma flag que significa, não há combustiveis, essa flag é usada mais tarde
    /// por outra função que monta o pacote de retorno

    if(resposta.distancia == -1) {
        snprintf(mensagem, len, "NOT");
        return;
    }


    printf("+++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("Resultado da Busca: \n");
    printf("\t\t Distancia do posto: %.2lf \n", resposta.distancia);
    printf("\t\t Menor preço %.3lf \n", (float)resposta.preco / 1000);
    printf("+++++++++++++++++++++++++++++++++++++++++++++++\n\n\n");


    // Coloca por referencia o conteudo da mensagem que sera enviada para o cliente.
    snprintf (
        mensagem,
        len,
        "O Posto de combustivel mais proximo de ti esta localizado a %.2lfKm e se encontra nas coordenadas: (%lf;%lf) com valor de R$%.3lf%c",
        resposta.distancia,
        resposta.latitude,
        resposta.longitude,
        (float)resposta.preco / 1000,
        '\0'
    );

}



/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função faz a impressao do pacote
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void imprimePacote(Frame *frame_recebido) {
    printf("===========================================================\n");
    printf("Pacote Recebido: \n");
    printf("\tTipo ======> %d\n", frame_recebido->tipo);
    printf("\tMensagem ======> %s", frame_recebido->pacote.string);
    printf("\tChecksum ======> %d\n", frame_recebido->checksum);
    printf("===========================================================\n\n\n");

    return;
}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função escreveArquivo, recebe a mensagem que deve ser escrita e uma variavel chamada
 * url, onde se encontra o caminho do arquivo, se der erro na apertura a função retorna null
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void escreveArquivo(const char *mensagem, const char *url) {

    FILE *arquivo = NULL;
    arquivo = fopen(url, "a");

    if(arquivo == NULL) {
        perror("Opa, alguma coisa aconteceu na abertura do arquivo :( \n");
        return;
    }

    fprintf(arquivo, "%s\n", mensagem);
    fclose(arquivo);
}




/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * A função salvar_dados salva os dados enviado do cliente para o servidor
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void salvar_dados(Frame frame_recebido) {
    printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Comando do cliente: SALVAR DADOS $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n\n");

    int tipoCombustivel;
    char mensagem[50];
    normalizaPacoteDeDados(frame_recebido, &tipoCombustivel, mensagem, 50);

    printf("tipo combustivel: %d\n", tipoCombustivel);
    printf("mensagem: %s\n", mensagem);

    if(tipoCombustivel == 0)
        escreveArquivo(mensagem, COMBUSTIVEL_DIESEL);
    if(tipoCombustivel == 1)
        escreveArquivo(mensagem, COMBUSTIVEL_ALCOOL);
    if(tipoCombustivel  == 2)
        escreveArquivo(mensagem, COMBUSTIVEL_GASOLINA);
}




/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * A função [trata_solicitacao] faz o tratamento de erros de mensagens invalidas, faz o controle
 * de checksum para saber se a mensagem esta correta, faz o redirecionamento para PESQUISA ou DADOS
 * conforme o cliente solicitou, e por fim envia ack para cliente.
 *
 * A função recebe 5 Argumentos, primeiro o frame_recebido, como o proprio nome diz, é uma struct onde
 * contem os dados recebidos do cliente, segundo é o socket no servidor a qual o cliente esta associado,
 * terceiro é a struct do cliente que envio a mensagem, quarto é a variavel de controle proximaSeq, passada por
 * argumento, nessa variavel contem o valor do pacote que o servidor deve receber, e por fim no quinto parâmetro
 * uma struct para enviar os dados ao cliente.
 *
 * O quinto parâmetro, frame_enviar é passado por argumento, porque ele pode ser usado tanto na função inicia_cliente
 * quanto na função trata_solicitacao, o caso de uso da struct frame_enviar na função inicia_cliente, é em que o
 * proximaSeq não é o esperado, então a propria função encaminha o mesmo pacote gerado, que esta na struct frame_enviar.
 * Por esse motivo ela é passada por parâmetro.
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void trata_solicitacao(Frame *frame_recebido, int socketServidor, sock_In cliente, int *proximaSeq, Frame *frame_enviar) {

    int tipo_operacao = trata_mensagem(frame_recebido->pacote.string);
    uint32_t check;
    char mensagem[150];

    imprimePacote(frame_recebido);

    check = checksum_verifica(frame_recebido->pacote.string, strlen(frame_recebido->pacote.string));
    printf("Valor do checksum no servidor: %d\n\n", check);

    // Se o valor do checksum_verification for diferente do valor recebido no campo checksum da struct recebida, quer dizer
    // que o pacote veio corrompido, reenviamos o pacote e retornamos sem continuar o codigo
    if(check != frame_recebido->checksum) {
        printf("[PACOTE VEIO CORROMPIDO]\n");

        sendto(
            socketServidor,
            (const char *)&(frame_enviar),
            sizeof(Frame),
            MSG_CONFIRM,
            (sock_addr*)&cliente,
            sizeof(cliente)
        );

        printf("Reenviando o pacote recebido\n");
        return;
    }


    // Se tipo_operacao for igual a um, significa que o cliente quer salvar dados
    if(tipo_operacao == 1 ) {
        salvar_dados(*frame_recebido);
        printf(":::::::::::::: Enviando mensagem de confirmação para o cliente\n");
        strcpy(mensagem, "Dados Salvos!\n");
    }

    if(tipo_operacao == 0) {
        pesquisa_dados(*frame_recebido, mensagem, sizeof(mensagem));
        printf(":::::::::::::: Enviando pacote com resposta para cliente\n");
    }

    if(tipo_operacao != 0 && tipo_operacao != 1) {
        printf("Caracteres errados\n");
        strcpy(mensagem, "Entrada invalida\n");
    }

    // Envia [ACK] Para cliente.
    enviaResposta(frame_recebido, socketServidor, cliente, mensagem, proximaSeq, frame_enviar);

}




/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função retorna -1 caso o primeiro e unico parâmetro seja diferente de um inteiro sem sinal
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
unsigned int trateEntrada(int argc, char **argv) {
    if(argc != 2) {
        printf("\033[7;30mFormato %s <porta>\033[m \n", argv[0]);
        exit(-1);
    }

    return atoi(argv[1]);
}




/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * A Função [inicia_cliente], recebe dois parâmetros, uma struct com o cliente solicitante, e um segundo
 * argumento porta, essa porta é passada porque sera estabelecido um par 1 para 1 entre o cliente que solicitou
 * a "conexão" e o novo socket criado.
 *
 * A partir do momento em que a pré configuração (criação do socket, bind com servidor) é feita, o servidor ficara no while
 * infinito aguardando mensagens do cliente. O processo exclusivo estabelecido entre o cliente solicitante e servidor
 * é fechada apos o cliente enviar uma mensagem sair para o servidor
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void inicia_cliente(sock_In *cliente, int port) {

    int dados, novo_socket ;
    socklen_t tamanho_cliente;

    Frame novo_frame_recebido, frame_enviar;
    sock_In  servidor;

    tamanho_cliente = sizeof(*cliente);


    novo_socket = criaSock();
    bindSock(&servidor, novo_socket, port);
    printf("\n\nConexão do cliente na porta %d estabelecida com sucesso, PID do processo: %d\n\n", port, getpid());

    int proximaSeq = 0;
    // A variavel [proximaSeq] serve para controlar no lado do servidor qual o proximo frame que deve ser recebido
    // ela é alterada somente se o pacote que foi recebido é o pacote certo, ela é alterada
    // via referencia em outra função, que faz o controle de envio de pacote, a função se chama enviaResposta

    while (true) {
        printf(":::::::::::::: Identificador do cliente: %d\n", getpid());
        puts(":::::::::::::: Aguardando mensagens...\n");

        // Fica escutando mensagem de somente um cliente, o cliente ao qual foi feito o bind.
        dados = recvfrom(
            novo_socket,
            (char *) &novo_frame_recebido,
            sizeof(Frame),
            MSG_WAITALL,
            (sock_addr *) &(*cliente),
            &tamanho_cliente
        );

        if (dados > 0) {

            /* Se o valor de tipo no frame recebido for diferente de proximaSeq quer dizer que o pacote é duplicado, ou seja
             * podemos descartar, e reenviar o mesmo ack para o cliente.
             *
             * Na primeira solicitação do cliente, a variavel proximaSeq estara com valor de 0, e é esperado do
             * Cliente, 0 no tipo tambem. Assim, estabelecido uma conexão e com a variavel de controle correta
             */

            if(novo_frame_recebido.tipo != proximaSeq) {
                printf("O Servidor já tem o pacote de ID %d, Reenviando o ACK do pacote %d\n", novo_frame_recebido.tipo, novo_frame_recebido.tipo);

                sendto(
                    novo_socket,
                    (const char *)&frame_enviar,
                    sizeof(Frame),
                    MSG_CONFIRM,
                    (sock_addr*)&(*cliente),
                    sizeof(*cliente)
                );

            } else // Se o proximaSeq é o esperado, envia a mensagem para tratamento.
                trata_solicitacao(&novo_frame_recebido, novo_socket, *cliente, &proximaSeq, &frame_enviar);

        }
    }
}




/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Função principal do programa, no primeiro parâmetro argc,
 * sera passado a porta a qual o servidor deve rodar
/*::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int main(int argc, char **argv) {
    int socketServidor, dados;
    unsigned porta;

    socklen_t tamanho_cliente;
    // typedef struct sockaddr_in sock_In é um typedef para facilitar a escrita
    sock_In servidor, cliente;

    Frame frame_recebido; // struct onde os dados recebidos serão ser armazenados
    porta = trateEntrada(argc, argv);// Faz o tratamento dos argumentos de entrada e retorna numero da porta

    socketServidor = criaSock(); // Cria socket
    bindSock(&servidor, socketServidor, porta); // Faz o bind do socket com a porta passada por argumento


    /* O socket criado servira para todas as conexões futuras,
     * qualquer mensagem recebida de um cliente x, passara
     * por esse socket. Logo apos, o servidor ira criar um novo processo
     * onde é criado um socket e variável exclusivas daquele processo
     * */

    tamanho_cliente = sizeof(cliente);
    printf("Servidor on\n");
    while (true) {
        // A função [recvfrom] nesse ponto vai receber conexão de qualquer cliente que saiba o endereço do servidor
        dados = recvfrom (socketServidor, (char *)&frame_recebido, sizeof(Frame), 0, (sock_addr*) &cliente, &tamanho_cliente);

        // Se for maior que zero, uma nova mensagem chegou.
        if(dados > 0)  {
            puts(":::::::::::::::::::: Um cliente acaba de fazer uma solicitação de conexão... Criando novo processo ::::::::::::::::::::\n");

            pid_t pid = fork(); // Cria processo novo para o cliente

            if(pid == 0) { // Processo é o filho, então o cliente se comunicara somente com ele.
                inicia_cliente(&cliente, gera_e_envia_porta(socketServidor, cliente)); // Estarta novo cliente
                close(socketServidor);
                exit(0); // Exit 0, saiu com sucesso
            }

            if(pid < 0) {
                printf("Erro na criação do processo\n");
                close(socketServidor);
                exit(-2); // Exit 2, criação de processo falhou
            }
        }

    }

}
