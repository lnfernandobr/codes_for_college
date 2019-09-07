#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <sstream>
#include <string.h>

int click = 0;
using namespace std;

#define HEIGHT 480
#define WIDTH 640
            int count = 0;

float V1[3] = {0};
float V2[3] = {0};
float V3[3] = {0};


int keyboard_event_x = 0;
int keyboard_event_y = 0;
double objx = 0, objy=0, objz=0;
double angle_x = 0, angle_y = 0;
double move_x = 0;
double move_y = 0;
double move_z = 0;
int move_cube = 0;


void setVector(float *V1, float *V2, unsigned int len) {
    for (unsigned int i = 0; i < len; ++i) 
        V1[i] = V2[i];
}

class Vetor {

    public:
        Vetor(float *V1, float *V2) {
            setVector(Va, V1, 3);
            setVector(Vb, V2, 3);
        }

    void drawVetor();
    void updateVetor(float move_x, float move_y, float move_z);

    private:
        float Va[3], Vb[3];
};
vector<Vetor>::iterator vetorIterator; // Iterator to traverse a Line array.
vector<Vetor> vetores;// Vector of lines.

void Vetor::updateVetor(float move_x, float move_y, float move_z) {
    

}

void Vetor::drawVetor() {
    
    glPushMatrix();
        glTranslatef(move_x, move_y, move_z);
        glBegin(GL_LINES);
        glColor3f(0, 51, 0);
            glVertex3f(Va[0], Va[1], Va[2]);
            glVertex3f(Vb[0], Vb[1], Vb[2]);
        glEnd();
    glPopMatrix();
}



void drawVetores(void) {
    // Loop through the lines array drawing each line.
    vetorIterator = vetores.begin();

    while(vetorIterator  != vetores.end()) {
        vetorIterator->drawVetor();
        vetorIterator++;
    }
}


/*THE FUNCTION TO DRAW THE STUFF ON THE SCREEN*/
void display( ) {
    
    printf("OPA MAN:\n");
    printf("Vetor 1: (%f %f %f)\n", V1[0], V1[1],  V1[2]);
    printf("Vetor 2: (%f %f %f)\n", V2[0], V2[1],  V2[2]);

   

    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
    int offset = 10;
    //The big green floor like polygon
    //The big green floor like polygon
    //The big green floor like polygon
    if (keyboard_event_x == 1){
        glRotatef(angle_x, 0.0, 1.0, 0.0);

    }
    else if (keyboard_event_y == 1){
        //glRotatef(angle_x, 0.0, 1.0, 0.0);
        glRotatef(angle_y, 1.0, 0.0, 0.0);
    }
    //glRotatef(angle_y, 1.0, 0.0, 0.0);
    glPushMatrix();
        glBegin( GL_LINES );
            glColor3f(0, 0, 1);
            glVertex3f( 0, 0,  100  );
            glVertex3f( 0, 0, -100  );
        glEnd( );
        glBegin( GL_LINES );
            glColor3f(1, 0, 0);
            glVertex3f( -80, 0,  0  );
            glVertex3f( 80, 0,  0  );
        glEnd( );
        glColor3f(0.8, 0.8, 0.8);
        for (int i = 0; i < 170 ; i+=10){
            glBegin( GL_LINES );
            glVertex3f( -80+i, 0,  100  );
            glVertex3f( -80+i, 0, -100  );
            glEnd( );
        }
        //glColor3f(0, 1, 0);
        for (int i = 0; i < 220 ; i+= 20){
            glBegin( GL_LINES );
            glVertex3f( -80, 0,  100-i  );
            glVertex3f(  80, 0,  100-i  );
            glEnd( );
        }
        if(click == 1) {

            glColor3f(0.8, 0.8, 0.8);
            glPushMatrix( );
                glColor3f( 1, 0, 0 );
                glPointSize(8);
                    glBegin(GL_POINTS);
                    glVertex3f(V1[0], V1[1], V1[2]);
                glEnd();
            glPopMatrix( );
        }
        drawVetores();

    glPopMatrix();

    glFlush( );
    glutSwapBuffers( );
}

void mouse( int button, int state, int x, int y) {
    double modelview[16], projection[16];
    int viewport[4];
    float z = 1 - 0.0001;
    //float z = 1 - 0.001;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {

            /*Read the projection, modelview and viewport matrices
            using the glGet functions.*/
            glGetDoublev( GL_PROJECTION_MATRIX, projection );
            glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
            glGetIntegerv( GL_VIEWPORT, viewport );

            keyboard_event_y = 0;
            keyboard_event_x = 0;

            move_x = 0;
            move_y = 0;
            move_z = 0;

            //Read the window z value from the z-buffer
            //glReadPixels( x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z );
            //if (z == 1) z -= 0.00001;
            //Use the gluUnProject to get the world co-ordinates of
            //the point the user clicked and save in objx, objy, objz.
            gluUnProject( x, viewport[3]-y, z, modelview, projection, viewport, &objx, &objy, &objz );
            cout << objx <<"," << objy <<"," << objz <<"," << z << endl;

            if(count == 4) {
               vetores.clear();
               count = 0;
            }

            if(click == 0) {
                V1[0] = objx;
                V1[1] = objy;
                V1[2] = objz;
                click++;
                count++;
            } else {
                V2[0] = objx;
                V2[1] = objy;
                V2[2] = objz;
                
                vetores.push_back(Vetor(V1, V2));
                click = 0;
                count++;
            }
            
            // gluUnProject( x, viewport[3]-y, 0, modelview, projection, viewport, &objx, &objy, &objz );
            // cout << objx <<"," << objy <<"," << objz <<"," << z << endl;

            
         
            
        }
    }
    glutPostRedisplay();
}
void keyboard( int key, int x, int y ) {

    if (key == GLUT_KEY_RIGHT){
        keyboard_event_x = 1;
        keyboard_event_y = 0;
        angle_x = 2.0;
        glutPostRedisplay();
    }

    else if(key == GLUT_KEY_LEFT){
        keyboard_event_x = 1;
        keyboard_event_y = 0;
        angle_x = -2.0;
        glutPostRedisplay();
    }

    else if(key == GLUT_KEY_UP){
        keyboard_event_y = 1;
        keyboard_event_x = 0;
        angle_y = 2.0;
        glutPostRedisplay();
    }

    else if (key ==  GLUT_KEY_DOWN) {
        keyboard_event_y = 1;
        keyboard_event_x = 0;
        angle_y = -2.0;
        glutPostRedisplay();
    }

}

void keyInput(unsigned char key, int x, int y) {
    
    keyboard_event_y = keyboard_event_x = 0;
    
    switch (key) {
        case 'x':
            move_x += 1;
            cout << "dentro" << endl;
            break;

        case 'X':
            move_x -= 1;
            break;

        case 'y':
            move_y += 1;
            break;

        case 'Y':
            move_y -= 1;
            break;

        case 'z':
            move_z += 1;
            break;

        case 'Z':
            move_z -= 1;
            break;

  }
  glutPostRedisplay();
}

void init( int width, int height ) {
    glClearColor( 0.3, 0.3, 0.3, 1 );
    glViewport( 0, 0, width, height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45, 1.33, 0.1, 400 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    gluLookAt( -50, 100, 250, 0, 0, 0, 0, 1, 0 );
}

int main( int argc, char **argv ) {
        glutInit( &argc, argv );
        //The most important part specify the things your
        //glut window should provide
        glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
        glutInitWindowSize( 1920, 1200 );
        glutCreateWindow("Teste usando gluUnProject");

        //enable z buffer
        glEnable( GL_DEPTH_TEST );
        //set the value in z-buffer as 1.0
        glClearDepth( 1.0 );
        init( 1920, 1200 );
        glutDisplayFunc( display );
        glutReshapeFunc( init );
        //glutIdleFunc( display );
        glutMouseFunc( mouse );
        glutKeyboardFunc(keyInput);
        glutSpecialFunc( keyboard );
        glutMainLoop( );
}