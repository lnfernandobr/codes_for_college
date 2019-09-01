#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define CADEIRAS 5                /* número de cadeiras para os clientes à espera */

const int unsigned true = 1;

sem_t customers;                /* número de cliente à espera de atendimento */
sem_t barbers;                  /* número de barbeiros à espera de clientes */
sem_t mutex;                    /* para exclusão mútua */
int esperando = 0;                /* clientes que estão esperando (não estão cortando) */

/* protótipos */
void* barber(void *arg);
void* customer(void *arg);

int main() {
    srand(time(NULL));

    sem_init(&customers, true, 0);
    sem_init(&barbers, true, 0);
    sem_init(&mutex, true, 1);

    pthread_t b, c;

    /* criando único barbeiro */
    pthread_create(&b, NULL, (void *) barber, NULL);


    /* criação indefinida de clientes */
    while(true) {
        pthread_create(&c, NULL, (void *) customer, NULL);
        sleep(1);
    }

    return 0;
}

void* barber(void *arg) {


    /*
     * Se
     * */
    while(true) {
        sem_wait(&customers);   /* vai dormir se o número de clientes for 0 */
        sem_wait(&mutex);       /* obtém acesso a 'esperando' */

        esperando--;  /*descresce de um o contador de clientes à espera */

        sem_post(&barbers);     /* um barbeiro está agora pronto para cortar cabelo */
        sem_post(&mutex);       /* libera 'esperando' */

        printf("Barbeiro estah cortando o cabelo de alguem!\n");
        sleep((rand() % 5) + 2);
    }

}

void *customer(void *arg) {

    /* Trava semaforo para região critica  */
    sem_wait(&mutex);

    if(esperando < CADEIRAS) {
        printf("Cliente chegou para cortar cabelo!\n");
        printf("Cliente sentou para esperar\n");
        esperando++;

        sem_post(&customers);   /* Acorda o barbeiro se o mesmo não estiver dormindo */
        sem_post(&mutex);       /* Libera região critica */

        // Esperando o barbeiro caso ele esteja trabalhando
        sem_wait(&barbers);
        printf("Cliente esta cortado o cabelo !\n");

    } else {
        sem_post(&mutex);       /* a barbearia está cheia; não espera */
        printf("####### Cliente desistiu! (O salao estah muito cheio!) ####### \n");
    }

    pthread_exit(NULL);
}

