#include <stdio.h>

int verify(int vet[], int count, int tam) {

    if(count == tam) {
        return vet[count];
    }

    int valor = verify(vet, count+1, tam);
    if(vet[count] > valor) {
        return vet[count];
    } else {
        return valor;
    }

};

int main() {
    int vet[] = {881,7,2,51,9,1851,1859};
    int response = verify(vet, 0, 7);
    printf("response = %d", response);

    return 0;
}