#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define N 5
#define PENSANDO 2
#define COM_FOME 1
#define COMENDO 0

/**
 *
 * esquerda:
 * 4 mod 5 = 4
 * 5 mod 5 = 0
 * 6 mod 5 = 1
 * 7 mod 5 = 2
 *
 * direita:
 * 1 mod 5 = 1
 * 2 mod 5 = 2
 * 3 mod 5 = 3
 * 4 mod 5 = 4
 *
 * */
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int filosofos[N] = { 0, 1, 2, 3, 4 };

const unsigned int true = 1;
const unsigned int false = 0;

sem_t mutex;
sem_t S[N];


void checar(int phnum) {

    // [ f1, f2, f3, f4]

    if (state[phnum] == COM_FOME && state[LEFT] != COMENDO && state[RIGHT] != COMENDO) {

        state[phnum] = COMENDO; // comendo

        sleep(rand() % 5);

        printf("Filosofo %d pega garfo %d e %d\n", phnum + 1, LEFT + 1, phnum + 1);

        printf("Filosofo %d esta comendo\n", phnum + 1);

        // acordar filósofos com fome durando o "larga_gargo"
        sem_post(&S[phnum]); // libera  outro filosofo
    }
}

void pega_garfo(int phnum) {

    sem_wait(&mutex); // trava semaforo

    state[phnum] = COM_FOME; // estado com fome

    printf("Filófoso %d está com fome\n", phnum + 1);


    checar(phnum); // checa se pode comer, se sim come se não volta

    sem_post(&mutex);

    /* Se tiver vizinhos comendo, espera para sinalizar */
    sem_wait(&S[phnum]);

    sleep(rand() % 5);
}

void larga_garfo(int phnum) {

    sem_wait(&mutex);

    state[phnum] = PENSANDO; // estado pensando

    printf("O filósofo %d colocou os garfos %d e %d na mesa\n", phnum + 1, LEFT + 1, phnum + 1);

    printf("Filófoso %d está pensando\n", phnum + 1);

    checar(LEFT);
    checar(RIGHT);

    sem_post(&mutex);
}
void* filosofo(void *num) { /* recebe id do filosofo */

    while (true) {

        int *i = num;

        /*
         * Espera por um tempo aleatório
         * Pega garfo
         * Espera mais um tempo aleatório
         * Devolve garfo
         * */

        sleep((rand() % 5));
        pega_garfo(*i);
        sleep((rand() % 5));
        larga_garfo(*i);
    }
}


int main()
{

    srand(time(NULL));
    pthread_t thread_id[N];

    /* Inicializando semaphore*/
    sem_init(&mutex, 0, 1);
    for (int unsigned i = 0; i < N; ++i)
        sem_init(&S[i], 0, 0);

    for (int unsigned i = 0; i < N; ++i) {
        pthread_create(&thread_id[i], NULL, (void*) filosofo, &filosofos[i]); /* Criando threads de filosofos*/
        printf("Filófoso %d está pensando\n", i + 1);
    }

    for (int unsigned i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);
}