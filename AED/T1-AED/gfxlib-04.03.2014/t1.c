#include "gfx.h"
#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep  */


void drawSquare(int n, int middleX,int middleY, int dimensionDrawing) {

    if(n < 0) return;

    /* As funções drawSquare são para desenhar os retangulos, é uma chamada recursiva, portanto é decremrentado
     * o [n] para que exista a condição de parada n < 0 return, as posições do middleX e middleY são recalculadas
     * dependendo de qual retangulo é imprimido, se o retangulo for imprimido na parte inferior do retangulo "pai"
     * ele ocupa + ou - 75% do retangulo pai, dependo da situação ele ocupa 25%.
     *
     * Para que todos os espaços parmanecam iguais, é subtraido ou adicionado ao retangulo "pai" 25% ou 75%
     * */

    drawSquare(
        n - 1,
        middleX - ((dimensionDrawing / 100.0) * 25),
        middleY - ((dimensionDrawing / 100.0) * 25),
        dimensionDrawing / 2
    );

    drawSquare(
        n - 1,
        middleX + ((dimensionDrawing / 100.0) * 75),
        middleY - ((dimensionDrawing / 100.0) * 25),
        dimensionDrawing / 2
    );

    drawSquare(
        n - 1,
        middleX - ((dimensionDrawing / 100.0) * 25),
        middleY + ((dimensionDrawing / 100.0) * 75),
        dimensionDrawing / 2
    );

    drawSquare(
        n - 1,
        middleX + ((dimensionDrawing / 100.0) * 75),
        middleY + ((dimensionDrawing / 100.0) * 75),
        dimensionDrawing / 2
    );

    /* Imprime o retangulo preenchido com a cor branca, as cordenadas middleX e middleY são passadas para a função,
     * esses valores são relativos e dinamicos, a função gfx_filled sabe as coordenadas de onde imprimir o retangulo
     * apartir desses valores, na primeira interação, o valor é middleX e middleY é calculado na função principal (main),
     * o valor é calculado pegando as cordenadas width e height da tela, divido ao meio e somado com o tamanho do desenho,
     * apartir desse ponto, as coordenadas são passados para as funções relativo a quem está chamando, isso significa que
     * se eu estou em uma chamada recursiva, o middleX e middleY vai ser o calculado com base em qual posição
     * o retangulo ese encontra
     */
    gfx_set_color(255, 255, 255);
    gfx_filled_rectangle(
            middleX,
            middleY,
            middleX + dimensionDrawing,
            middleY + dimensionDrawing
    );


    /* Aqui é imprimido o mesmo retangulo, mas a diferença é que imprimimos somente a linha para ele ficar com
    aspecto de sobreposição aos demais */
    gfx_set_color(0, 0, 0);
    gfx_rectangle(
            middleX,
            middleY,
            middleX + dimensionDrawing,
            middleY + dimensionDrawing
    );

    gfx_paint();
}
void initSquare(int n, int middleX, int middleY, int dimensionDrawing) {
    // Chama a função para imprimir os retangulos
    drawSquare(n , middleX, middleY, dimensionDrawing);
}


void drawTriangle(int n , int middleX, int middleY, int dimensionDrawing) {

    if(n < 0)
        return;


    gfx_set_color(255, 255, 255);

    gfx_line(
        middleX,
        middleY,
        middleX + dimensionDrawing,
        middleY
    );

    gfx_line(
        middleX,
        middleY,
        middleX + (dimensionDrawing / 2) ,
        middleY + dimensionDrawing
    );


    gfx_line(
        middleX + dimensionDrawing ,
        middleY,
        middleX + (dimensionDrawing / 2),
        middleY + dimensionDrawing
    );

    gfx_paint();


    drawTriangle(
            n - 1,
            middleX + (((dimensionDrawing /2) / 100.0) * 50) ,
            middleY - (((dimensionDrawing) / 100.0) * 50) ,
            dimensionDrawing / 2
    );


    drawTriangle(
        n - 1,
        middleX - (((dimensionDrawing /2) / 100.0) * 50) ,
        middleY + (((dimensionDrawing) / 100.0) * 50) ,
        dimensionDrawing / 2
    );

    drawTriangle(
            n - 1,
            middleX + (((dimensionDrawing) / 100.0) * 75) ,
            middleY + (((dimensionDrawing) / 100.0) * 50) ,
            dimensionDrawing / 2
    );


}
void initTriangle(int n, int middleX, int middleY, int dimensionDrawing) {
    // Chama a função para imprimir os triangulos
    drawTriangle(n , middleX, middleY, dimensionDrawing);
}


void menu() {
    int n, size, opc, buffer, middleX, middleY;

    printf("--------- MENU ---------\n");
    printf("1 - Imagem Retangular\n");
    printf("2 - Imagem Triangular\n");
    printf("3 - Sair.\n");
    printf("NOTA: ENTRADAS SOMENTE COM VALORES INTEIROS\n");
    printf("------------------------\n");
    scanf("%d", &opc);


    switch (opc) {
        case 1:

            printf("Quantos niveis de recursão você deseja ? \n");
            scanf("%d", &n);
            printf("Digite o tamanho do desenho em pixels: \n");
            scanf("%d", &size);

            middleX = (800 - size) / 2;
            middleY = (800 - size) / 2;

            gfx_init(800, 800,  "Imagem Retangular!");
            initSquare(n, middleX, middleY, size);
            printf("\nDIGITE ALGUMA COISA PARA FECHAR A JANELA!\n");
            scanf("%d", &buffer);
            gfx_quit();
            menu();
            break;

        case 2:
            printf("Quantos niveis de recursão você deseja ? \n");
            scanf("%d", &n);
            printf("Digite o tamanho do desenho em pixels: \n");
            scanf("%d", &size);

            middleX = (800 - size) / 2;
            middleY = (800 - size) / 2;

            gfx_init(800, 800,  "Imagem Retangular!");
            initTriangle(n, middleX, middleY, size);

            printf("\nDIGITE ALGUMA COISA PARA FECHAR A JANELA!\n");
            scanf("%d", &buffer);
            gfx_quit();
            menu();
            break;

        case 3:
            return;

        default:
            return;

    }
}

int main(void) {
    menu();
    return 0;
}
