#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10

/* create thread argument struct for thr_function */
typedef struct _thread_data_t {
    int tid;
    double stuff;
}thread_data_t;

/* function thread */
void *thr_function(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    printf("Hello from thread thr_function - Thread ID: %d\n", data->tid);

    pthread_exit(NULL);
}


int main(void) {
    int rc;

    pthread_t thr[NUM_THREADS];

    /* create a thread_data_t argument array*/
    thread_data_t thr_data[NUM_THREADS];

    /* create threads */
    for (int i = 0; i < NUM_THREADS; ++i) {
        thr_data[i].tid = i;
        if((rc = pthread_create(&thr[i], NULL, thr_function, &thr_data[i]))) {
            fprintf(stderr, "Error: pthread_create, rc %d\n", rc);
            return EXIT_FAILURE;
        }
    }

    /* block until all threads complete */
    for (int i  = 0; i  < NUM_THREADS; ++i )
        pthread_join(thr[i], NULL);

    return EXIT_SUCCESS;
}