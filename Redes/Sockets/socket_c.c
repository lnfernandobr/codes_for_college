#include <stdio.h>
#include <sys/socket.h>
/* Essa biblioteca é necessária para usarmos inet_addr
   Permite converter um endereço IP em formato longo */
#include <arpa/inet.h>

int main() {
	struct sockaddr_in servidor;
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

	return 0;
}
