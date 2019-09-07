#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <sstream>
#include <string.h>

static GLsizei WIDTH, HEIGHT; /* OpenGL window size. */
using namespace std;

struct ptr {
    float x;
    float y;
    float z;
};

struct ptr  currentPoint;

int
    idOperation = 0,
    keyBoarEventX = 0,
    keyBoarEventY = 0,
    click = 0,
    count = 0;

float
    V1[3] = { 0 },
    V2[3] = { 0 },
    VectorAux2[3] = { 0 },
    VectorAux1[3] = { 0 };

double 
    angleX = 0,
    angleY = 0,
    moveX = 0,
    moveY = 0,
    moveZ = 0;


static void setVector(float *V1, float *V2, unsigned int len) {
    for (unsigned int i = 0; i < len; ++i)
        V1[i] = V2[i];
}

static void setVectorCoordinates(float *V, float x, float y, float z) {
    V[0] = x;
    V[1] = y;
    V[2] = z;
}

class Line {

    public:
        Line(float *V1, float *V2) {
            setVector(Va, V1, 3);
            setVector(Vb, V2, 3);
        }

    void drawVector();

    private:
        float Va[3], Vb[3];
};

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

vector<Line>::iterator LineIterator;
vector<Line> linesVector; /* Vector of lines */

void Line::drawVector() {
    glPushMatrix();
        glBegin(GL_LINES);
        glColor3f(0, 51, 0);
            glVertex3f(Va[0], Va[1], Va[2]);
            glVertex3f(Vb[0], Vb[1], Vb[2]);
        glEnd();
    glPopMatrix();
}

/* Função desenha linha em tempo real, a função é chamada a cada movimento do mouse,
 * no vetor Tmp fica as coordenadas atuais do mouse, no vetor VectorAux1 fica o primeiro
 * ponto clicado.
*/
void drawLineDynamic(void) {

    float Tmp[3];

    setVectorCoordinates(Tmp, currentPoint.x, currentPoint.y, currentPoint.z);
    Line *dynamicVector = new Line(VectorAux1, Tmp);

    dynamicVector->drawVector();


}

void drawVectors(void) {
    LineIterator = linesVector.begin();
    while(LineIterator  != linesVector.end()) {
        LineIterator->drawVector();
        LineIterator++;
    }
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

void drawScenario() {

    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 100);
    glVertex3f(0, 0, -100);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
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
            exit(0);

    glutPostRedisplay();

}
void makeMenu(void) {
    glutCreateMenu(PickOperation);
    glutAddMenuEntry("Conceito Vetor ", 1);
    glutAddMenuEntry("Soma de Vetores ", 2);
    glutAddMenuEntry("Produto de Vetores ", 3);
    glutAddMenuEntry("Produto Escalar entre Vetores", 4);
    glutAddMenuEntry("Sair", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void display() {
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    int offset = 10;

    if(keyBoarEventX == 1)
        glRotatef(angleX, 0.0, 1.0, 0.0);

    if(keyBoarEventY == 1)
        glRotatef(angleY, 1.0, 0.0, 0.0);

    glPushMatrix();
        drawScenario();

        if(click == 1)
            drawPointer(V1, 1);

        if(click == 1)
            drawLineDynamic();

        if(click == 2)
            drawPointer(V2, 2);

        drawVectors();
    glPopMatrix();


    glFlush( );
    glutSwapBuffers( );
}




void mouse(int button, int state, int x, int y) {

    double objX = 0, objY = 0, objZ = 0;

    if (button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {

            getCoordinatesReal(x, y, &objX, &objY, &objZ);

            if(count == 6)
                count = 0, linesVector.clear();

            if(click == 0) {
                setVectorCoordinates(V1, objX, objY, objZ);
                click++, count++;
            }

            else if(click == 1) {
                setVectorCoordinates(V2, objX, objY, objZ);
                click++, count++;
            }

            else if(click == 2) {
                linesVector.push_back(Line(VectorAux1, VectorAux2));
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

void keyInput(unsigned char key, int x, int y) {
    
    keyBoarEventY = keyBoarEventX = 0;
    
    switch (key) {
        case 'x':
            moveX += 1;
            cout << "dentro" << endl;
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

int main( int argc, char **argv ) {

    glutInit( &argc, argv );

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize(1920, 1200);
    glutCreateWindow("Trabalho 5");

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