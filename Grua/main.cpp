#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "GL/glut.h"
using namespace std;

#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1

#define ECHAP 27
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);

//function called on each frame
GLvoid window_idle();

 GLfloat angleAlph = 45;
 float myanglerest = 0.1;

void displayGizmo(float ancho, float largo)
{
	glBegin(GL_LINES);
	glColor3f(0.255f,0.0f,0.0f);
	glVertex2d(0, 0);
	glVertex2d(ancho +5, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, 0);
	glVertex2d(0, largo+5);
	glEnd();
}

void dibujaRectangulo(float largo, float ancho, int gizmo=1) ///base x altura
{
    glBegin(GL_QUADS);
    glVertex2f(0, -ancho / 2.0f);
    glVertex2f(largo, -ancho / 2.0f);
    glVertex2f(largo, ancho/ 2.0f);
    glVertex2f(0, ancho/ 2.0f);
    glEnd();
    if(gizmo == 1){
        displayGizmo(largo,ancho);
    }
}

void dibujarLine(float tam){
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(0, -tam);
    glEnd();
}

void dibujaGrua(int ang1, int ang2,int largoCuerda)
{
    glTranslated(150,150,0);
    float anchoI= 4;
    float largoI= 5;
    float largoBrazo_A = 16;
    float anchoBrazo_A = 3;

    float largoBrazo_B = 5;

    if (ang2 >0) ang2 *= -1;
    //glPushMatrix();
    glColor3f(1.000, 0.271, 0.000);
    dibujaRectangulo(largoI,anchoI,0);
    //glPopMatrix();

    // brazo 1 amarelo
    glTranslated(largoI/2,anchoI/2,0);
    glRotated(ang1,0,0,1);
    glColor3f(1.000f, 0.843f, 0.000f);
    dibujaRectangulo(largoBrazo_A,anchoBrazo_A,0);

    // brazo 2 verde
    glTranslated(largoBrazo_A,0,0);
    glRotated(ang2,0,0,1);
    glColor3f(0.196f, 0.804f, 0.196f);
    dibujaRectangulo(largoBrazo_B,anchoBrazo_A,0);

    //Cuerda
    glTranslated(largoBrazo_B,0,0);
    glRotated(-ang1-ang2+angleAlph,0,0,1);
    glColor3f(1.0f,1.0f,1.0f);
    dibujarLine(largoCuerda);

    //Caja roja
    glColor3f(1.000f, 0, 0);
    glTranslated(-1,-largoCuerda-1,0);
    dibujaRectangulo(2,2,0);

    if(angleAlph >=45){
        myanglerest=-0.5;
    }else{
        if(angleAlph<=-45){
            myanglerest=0.5;
        }
    }
    angleAlph+=myanglerest;

    cout<<angleAlph<<endl;

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TP 2 : Grua move");


	initGL();
	init_scene();

	glutDisplayFunc(&window_display);

	glutReshapeFunc(&window_reshape);

	glutKeyboardFunc(&window_key);

	//function called on each frame
	glutIdleFunc(&window_idle);

	glutMainLoop();

	return 1;
}



GLvoid initGL()
{
	GLfloat position[] = { 0.0f, 5.0f, 10.0f, 0.0 };

	//enable light : try without it
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	//light 0 "on": try without it
	glEnable(GL_LIGHT0);

	//shading model : try GL_FLAT
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	//enable material : try without it
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(RED, GREEN, BLUE, ALPHA);
}



GLvoid window_display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-40,40,-40,40,-40,40);
	glOrtho(0,300,0,300,-1.0,1.0);

	/**dibujar aqui*/

    dibujaGrua(35,84,8);

	glutSwapBuffers();

	glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);

	glMatrixMode(GL_MODELVIEW);
}



void init_scene()
{

}

GLvoid window_key(unsigned char key, int x, int y)
{
	switch (key) {
	case ECHAP:
		exit(1);
		break;

	default:
		printf("La touche %d non active.\n", key);
		break;
	}
}


//function called on each frame
GLvoid window_idle()
{


	glutPostRedisplay();
}
