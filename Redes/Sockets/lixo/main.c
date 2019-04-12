#include <netdb.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <string.h>

int main(void) {
    char *nameHost = "www.google.com.br";
    char ip[100];

    struct hostent *machine;
    struct in_addr **addr_list;

    int i;


    /*
        Para se conectar a um host remoto é necessario obeter o endereço ip
        a função getHostByName é usada para este proposito, Ela pega o nome do dominio com parametro e retorna 
        a estrutura do tipo hostent
        Esta estrutura tem a informação do ip. a Blibioteca é a netdb.h
    */
    
    machine = gethostbyname(nameHost);
    if(machine == NULL) {
        herror("gethostbyname");
        return 1;
    }

    /*  Aqui daremos um cast em h_addr_list para in_addr
        Uma vez que h_addr_list também tem o endereço IP,
        mas spmente no formato longo.
    */
    addr_list = (struct in_addr **) machine->h_addr_list;

    for(i = 0; addr_list[i] != NULL; ++i) {
        // Retorna primeiro valor encontrado
        strcpy(ip, inet_ntoa(*addr_list[i]));
    }

    printf("Possui endereço ip: %s\n", nameHost, ip);

    return 0;
}