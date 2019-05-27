#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

int searchNumber(int idPID, int length, int *array, int x) {
    for (int i = length; i < length + 200; ++i) {
        if(array[i] == x) {
            return 1;
        }
    }

    return 0;
}

int main(void)
{

    int value = 1, status, array[1000];
    pid_t mainPID = fork();


    for (int j = 0; j < 1000 ; ++j) {
        array[j] = j + 1;
    }

    if(mainPID > 0) { // pai

        int num, length = 0, x, findValue, findI;
        printf("Qual numero deseja procurar ?\n");
        scanf("%d", &num);
        pid_t arrayPid[5];

        for(int i = 0; i < 5; ++i) {
            arrayPid[i] = fork();

            if(arrayPid[i] == 0) {
                printf("Eu sou o filho %d e vou procurar no intervalo de %d até %d\n", getpid(), length, length + 200);
                x = searchNumber(i, length, array, num);
                if(x == 1) {
                    findValue = getpid();
                    findI = i;
                }
                length += 200;
            } else if(arrayPid[i] > 0) exit(0);

        }

        printf("Valor encontrado no filho %d\n e no indice %d do vetor de pid's\n", findValue, findI + 1);
    }

}