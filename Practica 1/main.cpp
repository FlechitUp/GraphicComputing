#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include "GL/glut.h"
#include <iostream>
#define KEY_ESC 27
#define PI 3.14159265

using namespace std;

//dibuja un simple gizmo
void displaySquare(int Xcenter, int Ycenter, float tamArista)
{
	glBegin(GL_LINE_LOOP);
	glColor3d(255,0,0);
	/*glVertex2d(0, 0);
	glVertex2d(1, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, 0);
	glVertex2d(0, 1);
	gl*End();*/
	glVertex2f(Xcenter-(tamArista/2),Ycenter+(tamArista/2));//left of window
    glVertex2f(Xcenter+(tamArista/2),Ycenter+(tamArista/2));//bottom of window
    glVertex2f(Xcenter+(tamArista/2),Ycenter-(tamArista/2));//right of window
    glVertex2f(Xcenter-(tamArista/2),Ycenter-(tamArista/2));//top of window
    glEnd();//end drawing of line loop
}

void displayCircle(double Xcenter, double Ycenter, double radio){
    int cantpoints = 100;
    double angulo = 360/cantpoints;
    glBegin(GL_LINE_LOOP);
	glColor3d(255,5,10);

    for(double alpha = 0 ; alpha < 360; alpha =alpha + angulo){
        //cout<<" alpha = "<<alpha<<endl;
        double x2 = radio*sin(alpha*PI/180)+Xcenter;
        double y2 = radio*cos(alpha*PI/180)+Ycenter;
        glVertex2f(x2,y2);
    }
    glEnd();
}

void displayCircles(int cantCircles, double radixReduction){  ///radixReduction es un porcentaje
    int Xcenter = -30;
    int Ycenter = 8 ;
    double radio = 15;
    double newRadio;
    for(int i = 0 ; i < cantCircles ; i++){
        displayCircle(Xcenter, Ycenter, radio);
        newRadio = radio-ceil(radio*radixReduction/100);
        Xcenter = Xcenter-(radio-newRadio);
        radio = newRadio;
        //Ycenter = Ycenter - (radio/2);
    }
}

void displayAdjacentCircles(int cantCircles, double radixReduction){  ///radixReduction es un porcentaje
    int Xcenter = -20;
    int Ycenter = -30 ;
    double radio = 15;
    double newRadio;
    for(int i = 0 ; i < cantCircles ; i++){
        displayCircle(Xcenter, Ycenter, radio);
        newRadio = radio-ceil(radio*radixReduction/100);
        Xcenter = Xcenter+(radio+newRadio);
        radio = newRadio;
    }
}

void displayAdjacentCirclesAngle(int cantCircles, double radixReduction, double angulo){  ///radixReduction es un porcentaje
    float Xcenter = 5;
    float Ycenter = -5 ;
    double radio = 10;
    double newRadio;
    double radianAngle = angulo*PI/180;

    for(int i = 0 ; i < cantCircles ; i++){
        displayCircle(Xcenter, Ycenter, radio);
        newRadio = radio-(radio*radixReduction/100);
        Xcenter = Xcenter+((radio+newRadio)*sin(radianAngle));
        Ycenter = Ycenter+((radio+newRadio)*cos(radianAngle));
       // cout<<"Distance "<<radio+newRadio<<endl;
        radio = newRadio;

    }
    /*glBegin(GL_LINES);
    glVertex2f(-10,-10);
    glVertex2f(Xcenter,Ycenter);
    glEnd();*/
}

//
//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glLoadIdentity();


	//dibuja el gizmo
	displaySquare(-40,40,11);        ///Ejerc. 1
	displayCircle(-20,35,11);       ///Ejerc. 2
	displayCircles(6,28);           ///Ejerc. 3
	displayAdjacentCircles(6,28);   ///Ejerc. 4
	displayAdjacentCirclesAngle(6,28,10); ///Ejerc. 5

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-50.0f,  50.0f,-50.0f, 50.0f, -1.0f, 1.0f);
	// todas la informaciones previas se aplican al la matrice del ModelView
	glMatrixMode(GL_MODELVIEW);
}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP1 OpenGL : hello_world_OpenGL"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);

	glutMainLoop(); //bucle de rendering

	return 0;
}
