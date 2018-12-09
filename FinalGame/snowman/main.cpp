#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include "RgbImage.h"
#include "game.h"


#define ECHAP 27

using namespace std;

void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void timer(int);
void initCamera();
void loadTextureFromFile(char *,int );
GLvoid window_idle();
gameController GameControll;

//int NumbTextures = 3;
GLuint texture[3];         // Storage For One Texture ( NEW )
char* filename[] = {"C:/Users/USUARIO/Documents/2018/02/Grafica/utilTexture/heart.bmp",
                    "C:/Users/USUARIO/Documents/GitHub/GraphicComputing/Utils/Tutorial SnowMan/snowman/rocks.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/snowman/rua.bmp"};
int NumbTextures = 3;
//Doll myDoll;
//scene myscene;

int position;

void init()
{
    glEnable(GL_TEXTURE_2D);
	glutFullScreen();
	float lpos[]= {1.0,3.0,4.0};
	float lamb[]= {0.4,0.4,0.4,1.0};
	float ldif[]= {1.0,1.0,1.0,1.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glLightfv(GL_LIGHT0,GL_POSITION,lpos);
	/// video 2
	glLightfv(GL_LIGHT0,GL_AMBIENT,lamb);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,ldif);
	/// endl video 2
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800,800);
	//glutInitWindowPosition(0, 0);
	glutCreateWindow("MS");

    ///Read textures
    for(int i = 0 ; i < NumbTextures ; i++)
    {
      loadTextureFromFile(filename[i],i);
    }

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	timer(0);
	///glutTimerFunc(0,timer,0);
	//glutIdleFunc(&window_idle);
	glutMainLoop();
	return 0;
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	initCamera();
	GameControll.startRun();
	glutSwapBuffers();
}			///156

void reshape(int w, int h) 	/// 158
{
	glLoadIdentity();
	glViewport(0,0,(GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLdouble)w/ (GLdouble)h,1, 400 ); ///100
	glMatrixMode(GL_MODELVIEW);
}

void initCamera()
{
	glTranslatef(0.0, -2.0, -7.0);
	glRotatef(20.0f , 1.0, 0.0, 0.0);
}

void loadTextureFromFile(char *filename,int index)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   RgbImage theTexMap( filename );
   if (theTexMap.successImgLoad == false )
   {
        texture[index] = 0;
   }
   else {
        glGenTextures(1, &texture[index]);                    // Create The Texture
   }
   glBindTexture(GL_TEXTURE_2D, texture[index]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Typical Texture Generation Using Data From The Bitmap
    //oscurece la pantalla
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 1, GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );

}

void timer(int value)
{
    //rotAngle += 0.1;
    GameControll.myDoll.proneAngle += 0.1;
   //proneHold +=0.1
    // 100 milliseconds
    glutTimerFunc(9, timer, 0);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'v'://char (32):
		if(!GameControll.gameStarted)
        {
            cout<<"Game started"<<endl;
			GameControll.startGame();
		}
		break;
	case 'w':
		GameControll.actions('w');
		break;
	case 's':
		GameControll.actions('s');
		break;
	case 'a':
		GameControll.actions('a');
		break;
    case 'd':
        GameControll.actions('d');
		break;
    case ECHAP:
		exit(1);
		break;
	default:
		break;
	}
}

///super acelerado
GLvoid window_idle()
{
	glutPostRedisplay();
}




