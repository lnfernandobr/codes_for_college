#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
/* Essa biblioteca é necessária para usarmos inet_addr
   Permite converter um endereço IP em formato longo */
#include <arpa/inet.h>

int main() {
	struct sockaddr_in servidor;
	char *mensagem, resposta_servidor[2000];
	int meu_socket;

	meu_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (meu_socket == -1) {
		printf("\nErro ao criar o socket!\n");
	} else {
		printf("\nSocket criado com sucesso!\n");
	}

	servidor.sin_addr.s_addr = inet_addr("64.233.185.94"); /* Endereço do Google */
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(80);

	/* Conectar a um servidor remoto */
	if (connect(meu_socket, (struct sockaddr *) &servidor, sizeof(servidor)) < 0) {
		puts("\nErro ao conectar!\n");
	}

	puts("\nConectado!\n");

	/* Enviando alguns dados */
	mensagem = "GET / HTTP/1.1\r\n\r\n";

	if (send(meu_socket, mensagem, strlen(mensagem), 0) < 0) {
		puts("\nFalha ao enviar\n");
		return 1;
	}

	puts("\nDados enviados\n");

	/* Recebendo resposta do servidor */
	if (recv(meu_socket, resposta_servidor, 2000, 0) < 0) {
		puts("\nFalha ao receber resposta\n");
	}

	puts("\nResposta recebida\n");
	puts(resposta_servidor);

	close(meu_socket);

	return 0;
}
