#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include "game.h"

#define ECHAP 27
using namespace std;

void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void timer(int);

float rotAngle;  ///Angulo q hace q rote el doll
float characterOffset;
float jumpOffset;	bool jumping;
float proneAngle;
bool proning; 	int proneHold = 0;  /// pronacion movimiento de manos
float shiftAngle;	int shifting;
bool gameStarted = false;

int position;

void init()
{
    glEnable(GL_TEXTURE_2D);
	glutFullScreen();
	float lpos[]= {1.0,3.0,4.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

void startGame()
{
	gameStarted = true ; 		/// 33
	rotAngle=0; jumpOffset=0; proneAngle =0 ; shiftAngle= 0;
	characterOffset = 0;
	position = 0;
	jumping = false; proning = false;
}

int main(int argc, char** argv)		//40
{
	glutInit(&argc,argv);
	glutInitWindowSize(800,800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("MS");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	timer(0); //glutTimerFunc(0,timer,0);
	glutMainLoop();
	return 0;
}

void initCharacter()  		/// 55
{
	if(rotAngle<180)
		rotAngle+=2;
	if(rotAngle==180)
		startRun();
	if (jumping)		///61
	{
		if(jumpOffset<=2.0)
			jumpOffset+=0.1;
		else
			jumping =false;
	}
	else
	{
		if(jumpOffset>0.0)
			jumpOffset-=0.1;
		if(jumpOffset<0.0)
			jumpOffset=0.0;
	}
	if(proning)							//75
	{
		if(proneAngle<=75.0)			/// 77
			proneAngle+=5;
		else if(proneAngle>75.0 && proneHold <15)
			proneHold++;
		else
			proning = false;
		if(jumpOffset>0.0)
		{
			jumpOffset-=0.4;
			if(jumpOffset<=0.0)
			{
				jumpOffset = 0;
				jumping = false;
			}
		}

	}
	else
	{
		if(proneAngle>0.0)
			proneAngle-=2.5;
		if(proneAngle<=0.0)
		{
			proneHold = 0;
			proneAngle = 0.0;
		}
	}
	if(shifting==1)
	{
		if(shiftAngle<=25)   ///deberia estar en la linea 106 segun el video
			shiftAngle++;
		else
			shifting=0;
	}
	else if(shifting==-1)
	{
		if(shiftAngle>=-25)	///  113
			shiftAngle--;
		else
			shifting = 0;
	}
	else
	{
		if(shiftAngle>0)
			shiftAngle-=2;
		if(shiftAngle<0)
			shiftAngle+=2;
	}
	if(position==1 && characterOffset<2.5)
		characterOffset+=0.15;
	else if (position ==-1 && characterOffset>-2.5)
		characterOffset-=0.15;
	else if (position==0 && characterOffset<0)
	{
		if (characterOffset >=0.15)
			characterOffset = 0.0;
		else
			characterOffset -=0.15;
	}
	else if (position==0 && characterOffset >0)
	{
		if (characterOffset <0.15)
			characterOffset= 0.0;
		else
			characterOffset -=0.15;
	}
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	initCamera();
	drawScene(-characterOffset);
	if(gameStarted)
	{
		initCharacter();
		drawCharacter(rotAngle, jumpOffset, proneAngle, shiftAngle);
	}
	glutSwapBuffers();
}			///156

void reshape(int w, int h) 	/// 158
{
	glLoadIdentity();
	glViewport(0,0,(GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLdouble)w/ (GLdouble)h,1, 100 );
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'v'://char (32):
		if(!gameStarted)
            cout<<"Game started"<<endl;
			startGame();
		break;
	case 'w':
		if(jumpOffset==0.0)
		{
			jumping = true;
			if(proning)
				proning = false;
		}
		break;
	case 's':
		if(proneAngle==0.0)
			proning = true;
		else if(jumpOffset!=0)
			proning = true;
		break;
	case 'a':
		shiftAngle =0.0;
		shifting =1;
		position--;
		if(position<-1)
			position=-1;
		break;
    case 'd':
        shiftAngle =0.0;
		shifting =1;
		position++;
		if(position>=-1)
			position=1;
		break;
    case ECHAP:
		exit(1);
		break;
	default:
		break;
	}
}

void timer(int value)
{
    //rotAngle += 0.1;
    proneAngle += 0.1;
   //proneHold +=0.1
    glutPostRedisplay();
    // 100 milliseconds
    glutTimerFunc(9, timer, 0);
}











