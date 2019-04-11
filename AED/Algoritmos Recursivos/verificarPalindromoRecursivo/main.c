#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *palindromo(char *string, int count, int tam) {

    char *false  = "Nao e palindromo";
    char *true = "e palindromo";

    if(count <= (tam/2)) {

        if(string[count] == string[tam-1])
            palindromo(string, count + 1, tam-1);
        else
            return false;

    } else return true;

};


int main() {
    char string[] = "subinoonibus";
    char *response = palindromo(string,0, 12);

    printf("\nPalindromo = %s", response);
}