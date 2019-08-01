#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(void)
{

    int value = 1, status;
    pid_t pid = fork();

    if(pid > 0) { // pai

        waitpid(pid, &status, 0);
        for (int i = 20; i < 30; ++i) {
            printf("Numero %d no pai, meu pai é %d e meu pid é %d\n", i + 1,  getppid(), getpid());
            sleep(2);
        }
        printf("Porcesso pai vai morrer, meu pai é %d e meu pid é %d\n", getppid(), getpid());

    } else if(pid == 0) { // filho

        pid_t newPid = fork();
        if(newPid == 0) {
            printf("Neto 1 foi criado\n");
            for (int i = 0; i < 10; ++i) {
                printf("Numero %d no neto1, meu pai é %d e meu pid é %d\n", i + 1,  getppid(), getpid());
                sleep(2);
            }
            printf("Neto1 vai morrer, meu pai é %d e meu pid é %d\n", getppid(), getpid());
        }

        waitpid(newPid, &status, 0);
        printf("Filho1 foi criado\n");
        for (int i = 10; i < 20; ++i) {
            printf("Numero %d no filho1, meu pai é %d e meu pid é %d\n", i + 1,  getppid(), getpid());
            sleep(1);
        }
        printf("Porcesso filho1 vai morrer, meu pai é %d e meu pid é %d\n",  getppid(), getpid());

    }
}