#include <stdio.h>
#include <sys/socket.h>

int main() {
	int meu_socket;

	meu_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (meu_socket == -1) {
		printf("\nErro ao criar o socket!\n");
	} else {
		printf("\nSocket criado com sucesso!\n");
	}

	return 0;
}
