#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *reverse(char *string, int count, int tam) {

    char aux;

    if(count <= (tam/2)) {
        aux = string[count];
        string[count] = string[tam-1];
        string[tam-1] = aux;

        reverse(string, count+1, tam-1);
    }

    return string;
};


int main() {
    char string[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    char *response = reverse(string,0, 7);
    printf("\nResponse = %s", response);
}