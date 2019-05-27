/*
    Multiplicação de Matrizes (com troca de mensagem send e recv socket)
    *O processo pai lê as matrizes armazenadas em arquivos externos;

    matrizA.txt
    4
    0 1 0 0
    0 0 1 0
    0 0 0 1
    1 0 0 0

    matrizB.txt
    1 1 1 1
    1 1 1 1
    1 1 1 1
    1 1 1 1

    *"Cria" N processos filhos para efetuar a multiplicação das matrizes; N é lido no início do arquivo matrizA.txt
    *Cada filho vai efetuar parte da multiplicação das matrizes e enviar uma mensagem ao pai com o resultado.
    *O pai aguarda o fim da execução de todos os filhos e "junta" os resultados dos filhos em um único arquivo de saída;

    resultado.txt
    1 1 1 1
    1 1 1 1
    1 1 1 1
    1 1 1 1

 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int N;



void multiplicacaoMatrix(int matA[][N], int matB[][N], int matR[][N], int i) {
    char tam[100];

    char name[100];
    int aux = 0;
    FILE *f;



    sprintf(name, "arquivo%d.txt", i);
    f = fopen(name, "r + w");


    for(int j = 0; j < 4; j++) {
        for(int k = 0; k < 4;k++) {
            aux = aux + (matA[i][k] * matB[k][j]);
        }

        fprintf(f, "%d ", aux);
        matR[i][j] = aux;
        aux = 0;

    }


    fclose(f);

}


int main() {



    FILE *arqA = fopen("matrizA.txt","r + w");
    FILE *arqB = fopen("matrizB.txt","r + w");


    // Le o valor N
    fscanf(arqA,"%d",&N);

    int matrizA[N][N];
    int matrizB[N][N];
    int matrizR[N][N];

    for(int x=0;x<N;x++){
        for(int y=0;y<N;y++){
            matrizR[x][y]=0;
        }
    }

    // Ler matriz A
    for(int i = 0; i < N; i++){
        for(int j = 0;j < N; j++){
            fscanf(arqA,"%d", &matrizA[i][j]);
        }
    }

    // Ler matriz B
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            fscanf(arqB, "%d", &matrizB[i][j]);
        }
    }


    pid_t pidPAI = fork();
    int status;

    if(pidPAI > 0) {
        pid_t pid[N];

        for(int i=0;i<N;i++) {
            pid[i] = fork();

            if(pid[i] == 0) {
                multiplicacaoMatrix(matrizA, matrizB, matrizR, i);
                printf("\nO Processo %d esta calculando a linha %i\n", getpid(), i);
                waitpid(pid[i],&status,0);
            } else exit(0);
        }

    }
    printf("AAH3213I\n");

    FILE *resposta;
    resposta = fopen("resultado.txt","w");


    for(int x=0;x<N;x++){
        for(int y=0;y<N;y++){
            printf("%d ",matrizR[x][y]);
            fprintf(resposta, "%d ", matrizR[x][y]);
            // if(y == 3)printf("\n");
        }
        // fprintf(resposta, "%d ", matrizR[x][y]);
        fputc('\n', resposta);
        printf("\n");
    }
    fclose(resposta);


    fclose(arqA);
    fclose(arqB);

    printf("\n");
    return 0;
}