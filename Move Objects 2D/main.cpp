
#include "GL/gl.h"
#include "GL/glut.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

#define NP 4000
#define AGE 200

float winWid,winHeight; //Variables que se usaran mas adelante para almacenar los datos claves que serviran para el mouse
float rx,ry;//variables que serviran para dar ciertas coordenadas

void redraw( void )//Este metodo nos servira para redibujar nuestra figura
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,0.0);
	glRectf(-10.0+rx,-10.0+ry,10.0+rx,10.0+ry);//cambio de cooredenadas del rectangulo
	glutSwapBuffers();
}

void motion(int x, int y)//este metodo realizara las funciones para darle coordenadas a nuestro rectangulo al mover el mouse
{
    cerr << "motion. x:" << x << ", y:" << y << "\n";
    rx = float(x);
    ry = winHeight - float(y);
}

void mousebutton(int button, int state, int x, int y)//metodo para dar moviemiento a traves del mouse
{
	cerr << "mousebutton. x:" << x << ", y:" << y << "\n";
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		rx = float(x);
		ry = winHeight - float(y);
	redraw();
	}
}

int main(int argc, char *argv[])
{
	winWid = 600.0;
	winHeight = 400.0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(" ");
	glutPositionWindow(200,100);
	glutReshapeWindow(int(winWid),int(winHeight));
	glClearColor(0.0,0.0,0.0,1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,winWid,0.0,winHeight, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutDisplayFunc(redraw);
	glutIdleFunc(redraw);
	glutMotionFunc(motion);//manda llamar el metodo motion
	glutMouseFunc(mousebutton);//funcion exclusiva para el mouse
	glutMainLoop();

	return 0;
}
