#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <string.h>

/*
 * 1 - Ao entrar no programa, clique com botão direito do mouse para abrir o menu.
 * 2 - Escolha uma das opções.
 * 3 - O cenario mudara, de um clique na tela para adicionar um ponto. Você podera deslocar esse ponto na tela com as teclas
 * do teclado.
 * TECLAS: x, y, z = desloca x, y e z positvo
 * TECLAS: X, Y, Z = desloca no X, Y e Z negativo
 * 4 - Você pode rotacionar o cenario com as setas do teclado
 * 5 - Apos o 1 clique o ponto sera adicionado, já no segundo clique, sera mostrado um novo ponto, desloque-o para onde desejar e de mais
 * um clique para confirmar, o vetor será formado. faça isso e crie dois vetores na tela
 * 6 - Apos os 2 vetores estarem na tela, pressione botao direito do mouse e selecione calcular, a tela será redesenhada com o valor
 * da operação.
 *
 * */
static GLsizei WIDTH, HEIGHT; /* OpenGL window size. */
using namespace std;

struct ptr {
    float x;
    float y;
    float z;
};

struct ptr  currentPoint;

int
        idOperation = 1,
        keyBoarEventX = 0,
        keyBoarEventY = 0,
        click = 0,
        count = 0,
        calculate = 0,
        viewCalc = 0,
        withOutScenario = 0;


float
        V1[3]         = { 0 },
        V2[3]         = { 0 },
        VectorAux2[3] = { 0 },
        VectorAux1[3] = { 0 },
        R[3]          = { 0 },
        A1[3]         = { 0 },
        A2[3]         = { 0 },
        scalar = 0;

double
        angleX = 0,
        angleY = 0,
        moveX = 0,
        moveY = 0,
        moveZ = 0;


void getCoordinatesReal(int x, int y, double *objectX, double *objectY, double *objectZ);
static void setVectorCoordinates(float *V, float x, float y, float z);
void crossProduct(float vect_A[], float vect_B[], float cross_P[]);
static void setVector(float *V1, float *V2, unsigned int len);
void mouse(int button, int state, int x, int y);
void keyInput(unsigned char key, int x, int y);
static void mousePassiveMotion(int x, int y);
void keyboard( int key, int x, int y);
void drawPointer(float *V, int type);
int main( int argc, char **argv);
void init(int width, int height);
void PickOperation(int id);
void drawLineDynamic(void);
void drawVectors(void);
void drawScenario();
void makeMenu(void);
void calcule(void);
void display();



class Line {

public:
    Line(float *V1, float *V2, float *cors) {
        setVector(Va, V1, 3);
        setVector(Vb, V2, 3);
        setVector(colors, cors, 3);
    }

    void drawVector();

public:
    float Va[3], Vb[3], colors[3];
};

vector<Line>::iterator LineIterator;
vector<Line> linesVector; /* Vector of lines */

void Line::drawVector() {
    glPushMatrix();

    glBegin(GL_LINES);
//    glColor3f(1, 1, 1);
    glColor3f( 0, 54, 0);

    glVertex3f(Va[0], Va[1], Va[2]);
    glVertex3f(Vb[0], Vb[1], Vb[2]);
    glEnd();
    glPopMatrix();
}

void getCoordinatesReal(int x, int y, double *objectX, double *objectY, double *objectZ) {

    int viewport[4];

    double
            modelView[16],
            projection[16],
            z = 1 - 0.0001;

    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject(
            x,
            viewport[3] - y,
            z,
            modelView,
            projection,
            viewport,
            objectX,
            objectY,
            objectZ
    );

}
static void setVectorCoordinates(float *V, float x, float y, float z) {
    V[0] = x;
    V[1] = y;
    V[2] = z;
}
static void setVector(float *V1, float *V2, unsigned int len) {
    for (unsigned int i = 0; i < len; ++i)
        V1[i] = V2[i];
}
static void mousePassiveMotion(int x, int y) {

    double objectX = 0, objectY = 0, objectZ = 0;

    getCoordinatesReal(x, y, &objectX, &objectY, &objectZ);

    currentPoint.x = objectX;
    currentPoint.y = objectY;
    currentPoint.z = objectZ;

    /* Se o usuario tiver transladado o canario, o flag keyboard é setado para 1, então
     * se o mesmo não for setado para 0 aqui, o canario translada a cada movimento
     * do mouse.*/
    keyBoarEventY = 0;
    keyBoarEventX = 0;

    glutPostRedisplay();
}
void crossProduct(float vect_A[], float vect_B[], float cross_P[]) {
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}
void sumVector(float vect_A[], float vect_B[], float sum_P[]) {
    sum_P[0] = vect_A[0] + vect_B[0];
    sum_P[1] = vect_A[1] + vect_B[1];
    sum_P[2] = vect_A[2] + vect_B[2];
}
float dotProduct(float vect_A[], float vect_B[]) {

    float product = 0;

    // Loop for calculate cot product
    for (int i = 0; i < 3; i++)
        product = product + vect_A[i] * vect_B[i];

    return product;
}
void drawPointer(float *V, int type) {
    glColor3f(0.8, 0.8, 0.8);


    glPushMatrix();

    glTranslatef(moveX, moveY, moveZ);
    glColor3f(1, 0, 0);
    glPointSize(8);

    glBegin(GL_POINTS);
    glVertex3f(V[0], V[1], V[2]);
    setVectorCoordinates(
            type == 1 ? VectorAux1 : VectorAux2,
            V[0] + moveX,
            V[1] + moveY,
            V[2] + moveZ
    );
    glEnd();
    glPopMatrix();
}
void PickOperation(int id) {


    if(id == 1)
        idOperation = 1;

    if(id == 2)
        idOperation = 2;

    if(id == 3)
        idOperation = 3;

    if(id == 4)
        idOperation = 4;

    if(id == 5)
        calculate = 1;

    if(id == 6)
        withOutScenario = 1;

    if(id == 7)
        withOutScenario = 0;

    if(id == 8)
        exit(0);

    glutPostRedisplay();

}
void drawLineDynamic(void) {

    /* Função desenha linha em tempo real, a função é chamada a cada movimento do mouse,
     * no vetor Tmp fica as coordenadas atuais do mouse, no vetor VectorAux1 fica o primeiro
     * ponto clicado.
    */
    float Tmp[3];
    float cors[3] = {0, 51, 0};
    setVectorCoordinates(Tmp, currentPoint.x, currentPoint.y, currentPoint.z);
    Line *dynamicVector = new Line(VectorAux1, Tmp, cors);

    dynamicVector->drawVector();


}
void drawVectors(void) {
    LineIterator = linesVector.begin();

    while(LineIterator  != linesVector.end()) {
        LineIterator->drawVector();

        LineIterator++;
    }
}
void makeMenu(void) {
    glutCreateMenu(PickOperation);
    glutAddMenuEntry("Conceito Vetor ", 1);
    glutAddMenuEntry("Soma de Vetores ", 2);
    glutAddMenuEntry("Produto de Vetores ", 3);
    glutAddMenuEntry("Produto Escalar entre Vetores", 4);
    glutAddMenuEntry("CALCULAR", 5);
    glutAddMenuEntry("Esconder cenario", 6);
    glutAddMenuEntry("Ativar cenario", 7);
    glutAddMenuEntry("Sair", 8);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void drawScenario() {

    if(withOutScenario)
        return;

    glLineWidth(2.0);

    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 100);
    glVertex3f(0, 0, -100);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1, 41, 0);
    glVertex3f(-80, 0, 0);
    glVertex3f(80, 0, 0);
    glEnd();

    glColor3f(0.8, 0.8, 0.8);
    for (int i = 0; i < 170 ; i += 10) {
        glBegin(GL_LINES);
        glVertex3f(-80 + i, 0, 100);
        glVertex3f(-80 + i, 0, -100);
        glEnd();
    }

    for (int i = 0; i < 220 ; i += 20) {
        glBegin(GL_LINES);
        glVertex3f(-80, 0, 100 - i);
        glVertex3f(80, 0, 100 - i);
        glEnd();
    }


    // Draw the co-ordinate axes.


}

void writeBitmapString(void *font, char *string) {
    char *c;
    for(c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}
char *itoa(int value, char *result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
void drawTextScala() {
    char msg[30];
    char message[60];

    itoa(abs(scalar), msg, 10);

    snprintf(message, 60, "(%.2f, %.2f, %.2f) * (%.2f, %.2f, %.2f)", A1[0], A1[1], A1[2], A2[0], A2[1], A2[2]);

    glColor3f(0.0, 255.0, 255.0);
    glRasterPos3f(-80.0, 80.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char *)message);


    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(0.0, 79.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char *)"Modulo do produto Escalar: ");

    glColor3f(255.0, 255.0, 255.0);
    glRasterPos3f(50.0, 78.0, 0.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, msg);

}



void calcule(void) {

    LineIterator = linesVector.begin();

    float
            AUX[3]  = { 0 };

    A1[0] = linesVector[0].Vb[0] - linesVector[0].Va[0];
    A1[1] = linesVector[0].Vb[1] - linesVector[0].Va[1];
    A1[2] = linesVector[0].Vb[2] - linesVector[0].Va[2];

    AUX[0] = linesVector[1].Va[0] - linesVector[0].Va[0];
    AUX[1] = linesVector[1].Va[1] - linesVector[0].Va[1];
    AUX[2] = linesVector[1].Va[2] - linesVector[0].Va[2];

    A2[0] =  linesVector[1].Vb[0] - AUX[0];
    A2[1] =  linesVector[1].Vb[1] - AUX[1];
    A2[2] =  linesVector[1].Vb[2] - AUX[2];

    linesVector[1].Va[0] = linesVector[0].Va[0];
    linesVector[1].Va[1] = linesVector[0].Va[1];
    linesVector[1].Va[2] = linesVector[0].Va[2];

    linesVector[1].Vb[0] = AUX[0];
    linesVector[1].Vb[1] = AUX[1];
    linesVector[1].Vb[2] = AUX[2];


    if(idOperation == 2)
        sumVector(A1, A2, R);

    if(idOperation == 3)
        crossProduct(A1, A2, R);

    if(idOperation == 4)
        scalar = dotProduct(A1, A2);

    viewCalc = 1;
    glutPostRedisplay();

}



void operationsWithVectors(void) {

    if(keyBoarEventX == 1)
        glRotatef(angleX, 0.0, 1.0, 0.0);

    if(keyBoarEventY == 1)
        glRotatef(angleY, 1.0, 0.0, 0.0);

    glPushMatrix();

    drawScenario();

    if(idOperation == 1) {

        glRasterPos3f(-50.0, 80.0, 0.0);
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*) "Ambos os vetores se encontram na origem (0,0,0)");

        glRasterPos3f(-47.0, 70.0, 0.0);
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*) "Ler instrucoes de uso no terminal ou no codigo");

        glPushMatrix();
        glLineWidth(10.0);
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0.0);
        glVertex3f(100, 60.0, -80.0);
        glColor3f(0.0, 12.0, 0.0);
        glutWireSphere(5, 10, 10);
        glEnd();


        glLineWidth(10.0);
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0.0);
        glVertex3f(-100, 60.0, 80.0);
        glEnd();

        glColor3f(255.0, 255.0, 255.0);
        glRasterPos3f(-100.0, 20.0, 80.0);
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*) "Vetor A  - vermelho");

        glRasterPos3f(100.0, 20.0, -80.0);
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, (char*) "Vetor B  - preto");

        glPopMatrix();

        return;
    }

    if(viewCalc == 1 && idOperation == 4) {
        drawTextScala();
        return;
    }


    if(viewCalc == 1) {
        glPushMatrix();

        glBegin(GL_LINES);
        glColor3f(255, 0, 0);
        glVertex3f(linesVector[0].Va[0], linesVector[0].Va[1], linesVector[0].Va[2]);
        glVertex3f(R[0], R[1], R[2]);
        glEnd();

        if(idOperation == 2) {

            glPushAttrib(GL_ENABLE_BIT);
            glLineStipple(5, 0x5555);
            glEnable(GL_LINE_STIPPLE);
            glBegin(GL_LINES);
            glColor3f(255, 255, 255);
            glVertex3f(linesVector[1].Vb[0], linesVector[1].Vb[1], linesVector[1].Vb[2]);
            glVertex3f(R[0], R[1], R[2]);

            glVertex3f(linesVector[0].Vb[0], linesVector[0].Vb[1], linesVector[0].Vb[2]);
            glVertex3f(R[0], R[1], R[2]);
            glEnd();
            glPopAttrib();
            glPopMatrix();
        }
    }

    if(calculate == 1)
        calculate = 0, calcule();

    if(click == 1)
        drawPointer(V1, 1);

    if(click == 1)
        drawLineDynamic();

    if(click == 2)
        drawPointer(V2, 2);

    drawVectors();
    glPopMatrix();
}
void display() {
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    int offset = 10;


    operationsWithVectors();


    glFlush();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {

    double objX = 0, objY = 0, objZ = 0;

    if (button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {

            getCoordinatesReal(x, y, &objX, &objY, &objZ);

            if(count == 6) {
                count = 0;
                viewCalc = 0;
                linesVector.clear();
            }
            if(click == 0) {
                setVectorCoordinates(V1, objX, objY, objZ);
                click++, count++;
            }

            else if(click == 1) {
                setVectorCoordinates(V2, objX, objY, objZ);
                click++, count++;
            }


            else if(click == 2) {
                float cors[3];
                cors[0] = rand() % 256;
                cors[1] = rand() % 256;
                cors[2] = rand() % 256;

                linesVector.push_back(Line(VectorAux1, VectorAux2, cors));
                click = 0, count++;
            }

            keyBoarEventY = 0;
            keyBoarEventX = 0;

            moveX = 0;
            moveY = 0;
            moveZ = 0;
        }
    }

    glutPostRedisplay();
}
void keyInput(unsigned char key, int x, int y) {

    keyBoarEventY = keyBoarEventX = 0;

    switch (key) {
        case 'x':
            moveX += 1;
            break;

        case 'X':
            moveX -= 1;
            break;

        case 'y':
            moveY += 1;
            break;

        case 'Y':
            moveY -= 1;
            break;

        case 'z':
            moveZ += 1;
            break;

        case 'Z':
            moveZ -= 1;
            break;

        default:
            break;

    }

    glutPostRedisplay();
}
void keyboard( int key, int x, int y) {

    if (key == GLUT_KEY_RIGHT) {
        keyBoarEventX = 1;
        keyBoarEventY = 0;
        angleX = 2.0;
        glutPostRedisplay();
    }

    else if(key == GLUT_KEY_LEFT) {
        keyBoarEventX = 1;
        keyBoarEventY = 0;
        angleX = -2.0;
        glutPostRedisplay();
    }

    else if(key == GLUT_KEY_UP) {
        keyBoarEventY = 1;
        keyBoarEventX = 0;
        angleY = 2.0;
        glutPostRedisplay();
    }

    else if (key ==  GLUT_KEY_DOWN) {
        keyBoarEventY = 1;
        keyBoarEventX = 0;
        angleY = -2.0;
        glutPostRedisplay();
    }

}
void init( int width, int height ) {
    WIDTH = width;
    HEIGHT = height;

    glClearColor( 0.3, 0.3, 0.3, 1 );
    glViewport( 0, 0, width, height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.33, 0.1, 400);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(-50, 100, 250, 0, 0, 0, 0, 1, 0);
}

void showMessage(void) {
    puts("1 - Ao entrar no programa, clique com botão direito do mouse para abrir o menu.\n"
         "2 - Escolha uma das opções.\n"
         "3 - O cenario mudara, de um clique na tela para adicionar um ponto. Você podera deslocar esse ponto na tela com as teclas\n"
         "do teclado.\n"
         "--- TECLAS: x, y, z = desloca x, y e z positvo\n"
         "--- TECLAS: X, Y, Z = desloca no X, Y e Z negativo\n"
         "4 - Você pode rotacionar o cenario com as setas do teclado\n"
         "5 - Apos o 1 clique o ponto sera adicionado, já no segundo clique, sera mostrado um novo ponto, desloque-o para onde desejar e de mais\n"
         "um clique para confirmar, o vetor será formado. faça isso e crie dois vetores na tela\n"
         "6 - Apos os 2 vetores estarem na tela, pressione botao direito do mouse e selecione calcular, a tela será redesenhada com o valor\n"
         "da operação.");
}
int main( int argc, char **argv ) {
    srand(time(NULL));

    glutInit( &argc, argv );

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize(1920, 1200);
    glutCreateWindow("Trabalho 5");

    showMessage();
    glEnable( GL_DEPTH_TEST );
    glClearDepth(1.0);
    init(1920, 1200);

    glutDisplayFunc(display);
    glutReshapeFunc(init);

    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mousePassiveMotion);

    glutKeyboardFunc(keyInput);
    glutSpecialFunc(keyboard);

    makeMenu(); // Create menu.
    glutMainLoop();
}