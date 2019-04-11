#include <stdio.h>


double factory (int n) {
    if(n <= 1) return  1;
    return  n * factory(n - 1);
};


int main (void) {

    int number;
    printf("Digite o numero: \n");
    scanf("%d", &number);

    const double  response = factory(number);
    printf("Fatorial de %d: %.2lf", number, response );

    return 0;
}