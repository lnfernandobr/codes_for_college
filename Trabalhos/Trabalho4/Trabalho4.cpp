#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// g++ -o trabalho4  Trabalho4.cpp -lglut -lGL -lGLU -lm
#define PI 3.14159265

#define ROWS 8  // Number of rows of esfera
#define COLUMNS 5 // Number of columns of esfera
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will

float A[3];
float B[3];

class Esfera  {
    public:
        Esfera();

        Esfera(float x, float y, float z, float r, unsigned char colorR, unsigned char colorG, unsigned char colorB);

        float getCenterX() { return centerX; }
        float getCenterY() { return centerY; }
        float getCenterZ() { return centerZ; }
        float getRadius()  { return radius; }

        unsigned char getAlpha() { return color[3]; }
        void setAlpha(int alp) { color[3] = alp; }

        void draw();

    public:
        float centerX, centerY, centerZ, radius;
        unsigned char color[4];
};

// construtor padrão da esfera
Esfera::Esfera() {
    centerX = 0.0;
    centerY = 0.0;
    centerZ = 0.0;

    radius = 0.0; // Indica se existe esfera

    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    color[3] = 0;
}

// Esfera constructor.
Esfera::Esfera(float x, float y, float z, float r, unsigned char colorR, unsigned char colorG, unsigned char colorB) {

    centerX = x;
    centerY = y;
    centerZ = z;

    radius = r;

    color[0] = colorR;
    color[1] = colorG;
    color[2] = colorB;
    color[3] = 255;
}

void Esfera::draw() {
    if (radius > 0.0) { // If asteroid exists.
        glPushMatrix();
        glTranslatef(centerX, centerY, centerZ);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_BLEND);
        glColor4ubv(color);
        glutWireSphere(radius, (int)radius * 2, (int)radius * 2);
        glPopMatrix();
    }
}

Esfera arrayEsferas[ROWS][COLUMNS]; // Global array of asteroids.

void setup(void) {

    // Initialize global arrayEsferas.
    for (int j = 0; j < COLUMNS; j++)
        for (int i = 0; i < ROWS; i++)


            /* If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot
             * which indicates that there is no asteroid there because the default's radius is 0.
             */
            if (rand() % 100 < FILL_PROBABILITY)
                if (COLUMNS % 2) // Odd number of columns.
                    arrayEsferas[i][j] = Esfera( 25.0 * (-COLUMNS/2 + j), 0.0, -40.0 - 30.0 * i, 8.0, 255, 166, 0);


    glEnable(GL_DEPTH_TEST);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}

void crossProduct(float vect_A[], float vect_B[], float cross_P[]) {

    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[0] * vect_B[2] - vect_A[2] * vect_B[0];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

float magnitude(float *vet) {
    return sqrt(pow(vet[0], 2) + pow(vet[1], 2) + pow(vet[2], 2));
}

// calculate shortest dist. from point to line
// calculate shortest dist. from point to line
float shortDistance(float line_point1[], float line_point2[], float point[]) {

    float AB[3], AC[3], R[3];

    AB[0] = line_point2[0] - line_point1[0];
    AB[1] = line_point2[1] - line_point1[1];
    AB[2] = line_point2[2] - line_point1[2];

    AC[0] = point[0] - line_point1[0];
    AC[1] = point[1] - line_point1[1];
    AC[2] = point[2] - line_point1[2];

    crossProduct(AB, AC, R);
    float area = magnitude(R);

    float CD = area / magnitude(AB);

    return CD;
}

void collision() {

    float P[3];

    for (int j = 0; j < COLUMNS; j++) {
        for (int i = 0; i < ROWS; i++) {

            P[0] =  arrayEsferas[i][j].getCenterX();
            P[1] =  arrayEsferas[i][j].getCenterY();
            P[2] =  arrayEsferas[i][j].getCenterZ();

            float dis = shortDistance(A, B, P);

            if(dis < arrayEsferas[i][j].radius) {
                arrayEsferas[i][j].color[0] = rand() % 256;
                arrayEsferas[i][j].color[1] = rand() % 256;
                arrayEsferas[i][j].color[2] = rand() % 256;
            }

        }
    }

}



void display(void) {

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glPushMatrix();
            glColor3f(255.0, 255.0, 255.0);
            glLineWidth(2.0);
            glBegin(GL_LINES);
                glVertex3f(A[0], A[1], A[2]);
                glVertex3f(B[0], B[1], B[2]);
            glEnd();
        glPopMatrix();

        collision();


        for (int j = 0; j < COLUMNS; j++)
            for (int i = 0; i < ROWS; i++)
                arrayEsferas[i][j].draw();

    glPopMatrix();

    glutSwapBuffers();
}

/* Change these values for a different transformation  */
void reshape(int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glRotatef(-20, 20.0, 0.0, 0.0);

    gluPerspective (60.0, (GLfloat) w/(GLfloat) h, 60.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void mouse(int button, int state, int x, int y) {


    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];
    GLint realy;  /*  OpenGL y coordinate position  */
    GLdouble wx, wy, wz;  /*  returned world x, y, z coords  */

    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                glGetIntegerv (GL_VIEWPORT, viewport);
                glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
                glGetDoublev (GL_PROJECTION_MATRIX, projmatrix); /*  note viewport[3] is height of window in pixels  */

                realy = viewport[3] - (GLint) y - 1;

                printf ("Coordinates at cursor are (%4d, %4d)\n", x, realy);
                gluUnProject (
                    (GLdouble) x,
                    (GLdouble) realy,
                    0.0,
                    mvmatrix,
                    projmatrix,
                    viewport,
                    &wx,
                    &wy,
                    &wz
                );

                printf ("World coords at z=0.0 are (%f, %f, %f)\n", wx, wy, wz);
                A[0] = wx;
                A[1] = wy;
                A[2] = wz;

                gluUnProject (
                    (GLdouble) x,
                    (GLdouble) realy,
                    1.0,
                    mvmatrix,
                    projmatrix,
                    viewport,
                    &wx,
                    &wy,
                    &wz
                );

                printf ("World coords at z=1.0 are (%f, %f, %f)\n", wx, wy, wz);
                B[0] = wx;
                B[1] = wy;
                B[2] = wz;
            }

            break;

        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                exit(0);
            break;

        default:
            break;
    }

    glutPostRedisplay();

}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
            break;
    }
}


int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DOUBLE  | GLUT_DEPTH);
    glutInitWindowSize (1200, 900);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);

    setup();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc (keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}