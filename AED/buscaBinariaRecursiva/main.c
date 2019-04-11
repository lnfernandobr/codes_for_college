#include <stdio.h>
#include <math.h>


int buscaBinaria(int *vet, int inicio, int fim, int x) {

    if(inicio <= fim) {
        int index = (inicio + fim) / 2;

        if(vet[index] == x)
            return x;
        else {
            if(vet[index] < x)
                return  buscaBinaria(vet, index + 1, fim, x);

            else
                return  buscaBinaria(vet, inicio, index - 1, x);
        }
    }   else
            return -1;

}

int main() {
    int vet[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int num = 0;

    while (num != -1) {
        scanf("%d", &num);
        int x = buscaBinaria(vet, 0 , 7, num);
        printf("X == %d", x);

    }

    return 0;
}