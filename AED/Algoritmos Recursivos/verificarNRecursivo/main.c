#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *true = "Achou";
char *false = "Nao achou";

char *searchX(int *vet, int tam, int n) {

    if(tam >  0 ) {
        if(vet[tam] != n) {
            return  searchX(vet, tam-1, n);
        }
        return true;

    } else {
        return  false;
    }
};

int main() {
    int vet[] = {1, 2, 3,7, 4, 5};
    char *response = searchX(vet, 5, -7);

    printf("\nProcurando o numero  -7:  = %s", response);
}