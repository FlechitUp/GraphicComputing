#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <iostream>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <stdio.h>
#include <stdlib.h>

//Compilar linux : g++ -std=c++11 tp4.cpp TextureManager.cpp  -lglut -lGL -lGLU -lfreeimage -o b


#include <FreeImage.h>
#include "TextureManager.h"
#include "RgbImage.h"

#define KEY_ESC 27
#define RED 0
#define GREEN 0
#define BLUE 0
#define ALPHA 1
#define PI 3.1415926535897932384626433832795
#define MaximumTheta 1.0
#define MinimumTheta 0.0009
#define MaximumRadious 10.5
#define MinimumRadious .9

using namespace std;

/*** GLOBAL VARIABLES */
bool fullScreenMode = true; // Full-screen or windowed mode?
// OpenGL utility Functions
GLint arbol;
static int posX = 0;    /// CUBE pos X
static int posY = 0;    /// CUBE pos Y
double windowPosX = 10;
double windowPosY = 30;
double windowWidth = glutGet(GLenum GLUT_SCREEN_WIDTH)-30;
double windowHeight = glutGet(GLenum GLUT_SCREEN_HEIGHT)-20;

GLuint  texture[5];         // Storage For One Texture ( NEW )

int width =  glutGet(GLenum GLUT_SCREEN_WIDTH);
int height =  glutGet(GLenum GLUT_SCREEN_HEIGHT);


GLfloat IncrementTheta = 0.05;
GLint WindowSizX=width,WindowSizY=height;

GLfloat EyePosition_X=2.0,EyePosition_Y=1.3,EyePosition_Z= 2.0;
GLfloat Radious=3.3,ProRadious=3.0,InitialTheta1=0.716,InitialTheta2=0.403;

GLfloat Center_X=0.0,Center_Y=0.0,Center_Z=0.0;
GLfloat Up_X=0.0,Up_Y=1.0,Up_Z=0.0;

enum { ClockWise,AntiClockWise };
enum {Theta1,Theta2};

float Near=.05, Far=10.0,fova = 50.0;

GLint direction = AntiClockWise;
GLint PressMovenent=Theta1;
float x, y, z;
GLUquadricObj *cone, *base, *qsphere, *cylinder;
GLUquadric* qobj;
GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };

char* filename[] = {"C:/Users/USUARIO/Documents/2018/02/Grafica/casawee2/Grass01.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/casaWe/wall.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/casaWee2/door.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/casawee2/roof2.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/casawee2/tree.bmp"};
float myTam = 0.3;   ///roof, building
float tam = 0.5;   ///roof, building
float hTriangle = 0.5;  ///ALTURA PARA TRIangulo de techo
float myp= 0.2;   ///door, window

///Normals
    ///Wall A y B
    vector<float> myWallANormal;
    vector<float> myWallBNormal;

    /// Door
    vector<float> doorNormal;
    ///Window
    vector<float> windowNormal;

    ///Lateral wall
    vector<float> rightWallNormal;
    vector<float> leftWallNormal;

    ///Lateral triangle wall
    vector<float> rightTriWallNormal;
    vector<float> leftTriWallNormal;

    ///Roof
    vector<float> rightRoofNormal;
    vector<float> leftRoofNormal;

    ///Grass
    vector<float> grassNormal;


/** END GLOBAL VARIABLES */

/**FUNCTION DECLARATION*/

void CalculationX_Y_Z();
void CalculationTH1();
void CalculationTH2();
void Movenent();
/** END FUNCTION DECLARATION*/


/*
* Read a texture map from a BMP bitmap file.
*/
void loadTextureFromFile(char *filename,int index)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   RgbImage theTexMap( filename );

   glGenTextures(1, &texture[index]);                    // Create The Texture
   glBindTexture(GL_TEXTURE_2D, texture[index]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Typical Texture Generation Using Data From The Bitmap

    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 1, GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );

}

void getNormalVector(float fVert1[], float fVert2[], float fVert3[],
                     float& fNormalX, float& fNormalY, float& fNormalZ)
{
   float Qx, Qy, Qz, Px, Py, Pz, vectUnit;

   Qx = fVert1[0]-fVert2[0];
   Qy = fVert1[1]-fVert2[1];
   Qz = fVert1[2]-fVert2[2];

   Px = fVert3[0]-fVert2[0];
   Py = fVert3[1]-fVert2[1];
   Pz = fVert3[2]-fVert2[2];

   fNormalX = (Py*Qz) - (Pz*Qy);
   fNormalY = (Pz*Qx) - (Px*Qz);
   fNormalZ = (Px*Qy) - (Py*Qx);

   vectUnit = sqrt(pow(fNormalX,2) +  pow(fNormalY,2)+ pow(fNormalZ,2));
   /// cout<<"vect U"<<vectUnit<<endl;
   fNormalX = fNormalX / vectUnit;
   fNormalY = fNormalY / vectUnit;
   fNormalZ = fNormalZ / vectUnit;
}

vector<float> getNormalVector(vector<float> fVert1, vector<float> fVert2, vector<float> fVert3)
{
   vector<float> vNORMAL;
   float Qx, Qy, Qz, Px, Py, Pz, vectUnit;

   Qx = fVert1[0]-fVert2[0];
   Qy = fVert1[1]-fVert2[1];
   Qz = fVert1[2]-fVert2[2];

   Px = fVert3[0]-fVert2[0];
   Py = fVert3[1]-fVert2[1];
   Pz = fVert3[2]-fVert2[2];

   vNORMAL.push_back((Py*Qz) - (Pz*Qy));
   vNORMAL.push_back((Pz*Qx) - (Px*Qz));
   vNORMAL.push_back((Px*Qy) - (Py*Qx));

   vectUnit = sqrt(pow(vNORMAL[0],2) +  pow(vNORMAL[1],2)+ pow(vNORMAL[2],2));
   /// cout<<"vect U"<<vectUnit<<endl;
   vNORMAL[0] = vNORMAL[0] / vectUnit;
   vNORMAL[1] = vNORMAL[1] / vectUnit;
   vNORMAL[2] = vNORMAL[2] / vectUnit;

   return vNORMAL;
}

void keyBoard(unsigned char key, int x, int y)
{
    switch(key)
    {

    case 27:
        exit(0);
    }
}

void specialKeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
        fullScreenMode = !fullScreenMode;         // Toggle state
        if (fullScreenMode)                       // Full-screen mode
        {
            windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
        }
        else                                             // Windowed mode
        {
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
        }
        break;
    }
}

void grass()
{
    double topWidth = 0.6;
    double topThick = 0.02;
    double legLen = 0.3;
    float tam = 0.5;

    glPushMatrix();
        glTranslated( 0, legLen, 0);
        glScaled( topWidth, topThick, topWidth);
        glNormal3f(grassNormal[0], grassNormal[1], grassNormal[2]);
        glBindTexture( GL_TEXTURE_2D, texture[0]);

        glBegin(GL_QUADS);
            // Back Face
            glTexCoord2f(3.0f, 0.0f); glVertex3f(-tam , -tam , -tam );
            glTexCoord2f(3.0f, 3.0f); glVertex3f(-tam , -tam , tam );
            glTexCoord2f(0.0f, 3.0f); glVertex3f( tam ,  -tam , tam );
            glTexCoord2f(0.0f, 0.0f); glVertex3f( tam , -tam , -tam );
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void roof()
{
    //float myTam= 0.3;
    glPushMatrix();
        //glScalef(0.5f,0.2f,0.5f);
        //glTranslated(0.0f,0.09f,0.05f);
        //glRotatef(25.0f,1,0,0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glBegin(GL_QUADS);
        // Top Face
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-myTam,  myTam, -myTam+0.2f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-myTam,  myTam,  myTam);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( myTam,  myTam,  myTam);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam+.2f);
        glEnd();
    glPopMatrix();
}

void building()
{
    //float myTam= 0.3;
    //float hTriangle = 0.5;  ///ALTURA PARA TRIangulo de techo

    ///Roof A
    GLfloat fVert1[3] = {-myTam, myTam,  myTam};
    GLfloat fVert2[3] = { myTam, myTam,  myTam};
    GLfloat fVert3[3] = { myTam, hTriangle, 0.0f};
    ///Roof B
    GLfloat fVertB1[3] = {  -myTam, myTam, -myTam};
    GLfloat fVertB2[3] = { -myTam, hTriangle, 0};
    GLfloat fVertB3[3] = { myTam, hTriangle, 0};

    glPushMatrix();
        glTranslated(0.4, 0.02, 0.4);

            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glTranslatef(0,0.25f,0);
            ///Walls
            glNormal3f(myWallANormal[0], myWallANormal[1], myWallANormal[2]);
            glBegin(GL_QUADS);
                // Front Face
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-myTam, -myTam,  myTam);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( myTam, -myTam,  myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam, myTam,  myTam);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-myTam,  myTam,  myTam);
            glEnd();

            glNormal3f(myWallBNormal[0], myWallBNormal[1], myWallBNormal[2]);
            glBegin(GL_QUADS);
                // Back Face
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-myTam, -myTam, -myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-myTam,  myTam, -myTam);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam);
                glTexCoord2f(0.0f, 0.0f); glVertex3f( myTam, -myTam, -myTam);
            glEnd();
            ///Lateral walls

            glNormal3f(rightWallNormal[0], rightWallNormal[1], rightWallNormal[2]);
            glBegin(GL_QUADS);
            // Right face
                glTexCoord2f(1.0f, 0.0f); glVertex3f( myTam, -myTam, -myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( myTam,  myTam,  myTam);
                glTexCoord2f(0.0f, 0.0f); glVertex3f( myTam, -myTam,  myTam);
            glEnd();

            glNormal3f(leftWallNormal[0], leftWallNormal[1], leftWallNormal[2]);
            glBegin(GL_QUADS);
                // Left Face
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-myTam, -myTam, -myTam);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-myTam, -myTam,  myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-myTam,  myTam,  myTam);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-myTam,  myTam, -myTam);
            glEnd();
            ///Lateral triangles walls
            glNormal3f(rightTriWallNormal[0], rightTriWallNormal[1], rightTriWallNormal[2]);
            glBegin(GL_TRIANGLES);
                // Right triangle wall
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam);///1
                glTexCoord2f(0.5f, 0.5f); glVertex3f( myTam, hTriangle, 0);///2R
                glTexCoord2f(0.0f, 1.0f); glVertex3f( myTam,  myTam,  myTam);///3
            glEnd();

            glNormal3f(leftTriWallNormal [0], leftTriWallNormal [1], leftTriWallNormal [2]);
            glBegin(GL_TRIANGLES);
                // Left triangle wall
                glTexCoord2f(1.0f, 1.0f); glVertex3f( -myTam, myTam,  myTam);///A
                glTexCoord2f(0.5f, 0.5f); glVertex3f( -myTam, hTriangle ,0);///2L
                glTexCoord2f(0.0f, 1.0f); glVertex3f( -myTam,  myTam,  -myTam);///B

            glEnd();

            ///Roof
            glBindTexture(GL_TEXTURE_2D, texture[3]);

            glNormal3f(rightRoofNormal[0], rightRoofNormal[1], rightRoofNormal[2]);
            glBegin(GL_QUADS);
                //Front roof
                glTexCoord2f(1.0f, 1.0f); glVertex3f( -myTam, myTam,  myTam);  ///A
                glTexCoord2f(0.0f, 1.0f); glVertex3f(  myTam, myTam,  myTam);  ///3
                glTexCoord2f(0.0f, 0.26f); glVertex3f(  myTam, hTriangle, 0.0f); ///2R
                glTexCoord2f(1.0f, 0.26f); glVertex3f( -myTam, hTriangle, 0.0f); ///2L
            glEnd();

            glNormal3f(leftRoofNormal[0], leftRoofNormal[1], leftRoofNormal[2]);
            glBegin(GL_QUADS);
                //Back roof
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam,  -myTam);  ///1
                glTexCoord2f(0.0f, 1.0f); glVertex3f( -myTam,  myTam, -myTam);  ///B
                glTexCoord2f(0.0f, 0.26f); glVertex3f(-myTam, hTriangle ,0);     ///2L
                glTexCoord2f(1.0f, 0.26f); glVertex3f( myTam, hTriangle, 0.0f);  ///2R
            glEnd();

    glPopMatrix();

}

void Arbol2a()
{
	float a=0.25;
	float b=0.5;
	///PAREDES
	glBegin(GL_QUADS);
        glVertex3f(-a,0,0);
        glTexCoord2f(0.0, 0.0);//coordenadas de textura

        glVertex3f(a, 0,0);
        glTexCoord2f(0.0, 1.0f);

        glVertex3f(a,b,0);
        glTexCoord2f(1.0, 1.0f);

        glVertex3f(-a,b,0);
        glTexCoord2f(1.0, 0.0);
	glEnd();

}

void Billboard()
{

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//funcion de transparencia
    glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, arbol);
        for(int i=0;i<180;i+=360/9)
		{
			glRotatef(i, 0.0, 1.0, 0.0);
			Arbol2a();
		}
    glDisable(GL_BLEND);
}

void theWindow()
{
    //float myp= 0.2;
    glNormal3f(windowNormal[0],windowNormal[1],windowNormal[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
        // Front Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-myp, -myp,  myp);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( myp, -myp,  myp);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( myp,  myp,  myp);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-myp,  myp,  myp);
    glEnd();
}

void theDoor()
{
    //float myp= 0.2;
    //vector<float> doorNormal;
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glNormal3f(doorNormal[0], doorNormal[1], doorNormal[2]);
    glBegin(GL_QUADS);
        // Front Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-myp, -myp,  myp);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( myp, -myp,  myp);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( myp,  myp,  myp);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-myp,  myp,  myp);
    glEnd();
}

void display(void)
{
    glViewport(0, 0, WindowSizX, WindowSizY );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectR = (float)WindowSizX / WindowSizY;

    gluPerspective(fova,aspectR, Near, Far);

    glMatrixMode(GL_MODELVIEW);/////
    glLoadIdentity();//////

    gluLookAt(EyePosition_X,EyePosition_Y,EyePosition_Z,Center_X,Center_Y,Center_Z,Up_X,Up_Y,Up_Z);

    ///start drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
        ///Light
        glPushMatrix ();

           glRotated ((GLdouble) posX,0.1, 0.0, 0.0);
           glLightfv (GL_LIGHT0, GL_POSITION, position);

           glTranslated (0.0, 0.0, 1.5);
           glDisable (GL_LIGHTING);

           glutSolidCube(0.1);
           glEnable (GL_LIGHTING);
        glPopMatrix ();

        /// Building Walls
        glPushMatrix();
            glTranslated(-1,0,-0.8);
            glScalef(1.5f, 1.0f,1.0f);
            building();
        glPopMatrix();

        //glDisable(GL_TEXTURE_2D);

        /// Grass
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glScalef(4,1,4);
            glTranslated(0,-0.33,0);
            grass();
        glPopMatrix();

        /// window
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glScalef(0.5f,0.5f,0.5f);
            glTranslatef(-0.3f,0.65f,-0.39f);
            theWindow();
        glPopMatrix();

        /// door final
        glPushMatrix();
            glScalef(0.6f,1.2f,0.5f);
            glTranslatef(-0.8f,0.175f,-0.399f);
            theDoor();
        glPopMatrix();

        glPushMatrix();
            glTranslated(0.7f, -0.01f, 0.4f);
        //glDisable(GL_LIGHT0);
            Billboard();

        glPopMatrix();
        //glEnable(GL_LIGHT0);
        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    glFlush();

    glutSwapBuffers();
}

void RepeatDisplay()
{
    glutPostRedisplay();
}

void Movenent()
{
    CalculationX_Y_Z();

    if(PressMovenent == Theta1)
        CalculationTH1();
    else
        CalculationTH2();

}

void CalculationX_Y_Z()
{
    ProRadious = Radious * cos(InitialTheta2);
    EyePosition_Y = Radious * sin(InitialTheta2);

    EyePosition_X = ProRadious * cos(InitialTheta1);
    EyePosition_Z = ProRadious * sin(InitialTheta1);

}

void CalculationTH1()
{
    if (InitialTheta1 > PI*2)
        InitialTheta1 = 0.0;
    if(direction == AntiClockWise)
    {
        InitialTheta1 += IncrementTheta;
        printf("InitialTheta1: %f\n",InitialTheta1);
    }
    else
        InitialTheta1 -= IncrementTheta;
    printf("InitialTheta1: %f\n",InitialTheta1);
}

void CalculationTH2()
{
    if (InitialTheta2 > PI*2)
        InitialTheta2 = 0.0;

    if(direction == AntiClockWise)
    {
        InitialTheta2 += IncrementTheta;
        printf("InitialTheta2: %f\n",InitialTheta2);
    }

    else
        InitialTheta2 -= IncrementTheta;
    printf("InitialTheta2: %f\n",InitialTheta2);

}

void CalculationRadious(unsigned int key)
{
    if(key == '-')
    {
        if(Radious > MaximumRadious)
            Radious = MaximumRadious;
        else
            Radious += 0.2;
        printf("Radious: %f\n",Radious);
    }
    else if(key == '+')
    {
        if(Radious <= MinimumRadious)
            Radious = MinimumRadious;
        else
            Radious -= 0.2;
        printf("Radious: %f\n",Radious);
    }

    CalculationX_Y_Z();
}

void KeybordFunction( unsigned char key, int x, int y )
{

    if(key=='-')

        CalculationRadious(key);

    else if(key=='+')
        CalculationRadious(key);

    else if(key == '4')
    {
        direction = ClockWise;
        PressMovenent = Theta1;
        Movenent();
    }

    else if(key == '6')
    {

        direction = AntiClockWise;
        PressMovenent = Theta1;
        Movenent();
    }
    else if(key == '8')
    {

        direction = ClockWise;
        PressMovenent = Theta2;
        Movenent();
    }
    else if(key == '2')
    {
        direction = AntiClockWise;
        PressMovenent = Theta2;
        Movenent();
    }
    else if(key == 'y')
    {
        //cout<<posY<<endl;
        posY = (posY + 1) % 360;
        //glutPostRedisplay();
    }
    else if(key == 27)  exit(0);

}

void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            posX = (posX + 30) % 360;
            glutPostRedisplay();
         }
         break;
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN) {
            posX = (posX - 30) % 360;
            glutPostRedisplay();
         }
         break;
      default:
         break;
   }
}

void setupLights()
{
  // Color values: red light.
  GLfloat Light0Amb[] = {0.75f, 0.75f, 0.75f, 1.0f};
  GLfloat Light0Dif[] = {0.9f, 0.9f, 0.9f, 1.0f};
  GLfloat Light0Spec[] = {0.4f, 0.4f, 0.4f, 1.0f};
  // Position values: puntual light.
  GLfloat Light0Pos[] = {0.0f, 20.0f, 20.0f, 1.0f};
  GLfloat direction[] = {0.0, -1.0, -1.0};

  // Light0 parameters.
  glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Dif);
  glLightfv(GL_LIGHT0, GL_SPECULAR, Light0Spec);
  glLightfv(GL_LIGHT0, GL_POSITION, Light0Pos);

  // Activate light.
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void initGL()
{
    glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);

	glClearColor(RED, GREEN, BLUE, ALPHA);
	////3
	glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLAT
	//glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Activa la corrección de perspectiva


	GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_defused[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_defused);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat lightIntensity[] = {1.7f, 1.7f, 1.7f, 1.0f};
    GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};

    //GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

}

void cargarWithPNG()
{
    arbol = TextureManager::Inst()->LoadTexture("C:/Users/USUARIO/Documents/2018/02/Grafica/testFreeimage/tree.png", GL_BGRA_EXT, GL_RGBA);
}

void calculateNormals(){

    myWallANormal = getNormalVector({-myTam, -myTam,  myTam} ,{myTam, -myTam,  myTam},{myTam, myTam,  myTam });
    myWallBNormal = getNormalVector({-myTam, -myTam, -myTam} ,{-myTam,  myTam, -myTam},{myTam,  myTam, -myTam });
    doorNormal = getNormalVector({-myp, -myp,  myp} ,{myp, -myp,  myp},{myp,  myp,  myp});
    grassNormal = getNormalVector({-tam , -tam , -tam } ,{-tam , -tam , tam },{tam ,  -tam , tam});
    windowNormal = getNormalVector({-myp, -myp,  myp}, {myp, -myp,  myp} ,{myp,  myp,  myp});
    rightWallNormal = getNormalVector({myTam, -myTam, -myTam} ,{myTam,  myTam, -myTam},{myTam,  myTam,  myTam});
    leftWallNormal = getNormalVector({-myTam, -myTam, -myTam} ,{-myTam, -myTam,  myTam},{-myTam,  myTam,  myTam});
    rightTriWallNormal = getNormalVector({myTam,  myTam, -myTam} ,{myTam, hTriangle, 0},{myTam,  myTam,  myTam});
    leftTriWallNormal = getNormalVector({-myTam, myTam,  myTam} ,{-myTam, hTriangle ,0},{-myTam,  myTam,  -myTam});
    rightRoofNormal = getNormalVector({-myTam, myTam,  myTam}, {myTam, myTam,  myTam} , {myTam, hTriangle, 0} );
    leftRoofNormal = getNormalVector({myTam, myTam, -myTam}, {-myTam, myTam,  -myTam} , {-myTam, hTriangle, 0} );
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutInitWindowSize(WindowSizX, WindowSizY);
    glutCreateWindow("3D Home");

    calculateNormals();
    initGL();
    cargarWithPNG();
    glutKeyboardFunc(KeybordFunction);

    glutIdleFunc(RepeatDisplay);

    qobj = gluNewQuadric();

    for ( int  i=0 ; i<5 ; i++ )
    {
        loadTextureFromFile(filename[i],i);
    }

    //setupLights();

    glutDisplayFunc(display);

    glShadeModel(GL_SMOOTH); /// modelo de shading try GL_FLAT
    //glEnable(GL_CULL_FACE);  ///no trata las caras escondidas
    glEnable(GL_DEPTH_TEST); /// Activa el Z-Buffer
    glDepthFunc(GL_LEQUAL); ///Modo de funcionamiento del Z-Buffer
    //glEnable(GL_NORMALIZE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); /// Activa la corrección de perspectiva

    glutSpecialFunc(specialKeys);
    glutFullScreen();
    glutMouseFunc(mouse);

    //Init();

    glutMainLoop();

    return 0;
}

