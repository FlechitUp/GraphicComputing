/////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

// C library headers
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cassert>
#include <windows.h>


// C++ headers
#include <iostream>
#include <string>
#include <random>

#include "RgbImage.h"


using namespace std;


bool fullScreenMode = true; // Full-screen or windowed mode?

// OpenGL utility Functions

double windowPosX = 10;
double windowPosY = 30;
double windowWidth = glutGet(GLenum GLUT_SCREEN_WIDTH)-30;
double windowHeight = glutGet(GLenum GLUT_SCREEN_HEIGHT)-20;

GLuint  texture[5];         // Storage For One Texture ( NEW )
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


std::random_device rd;
std::mt19937 gen(rd());

void randomColor()
{
    double colorR = generate_canonical<double, 2>(gen);
    double colorG = generate_canonical<double, 2>(gen);
    double colorB = generate_canonical<double, 2>(gen);
    glColor3f(colorR,colorG,colorB);
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//===========================================================================================



#define PI 3.1415926535897932384626433832795
#define MaximumTheta 1.0
#define MinimumTheta 0.0009
#define MaximumRadious 10.5
#define MinimumRadious .9

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

//RGBpixmap pix[15];

//FUNCTION DECLEARATION

void CalculationX_Y_Z();
void CalculationTH1();
void CalculationTH2();
void Movenent();



GLUquadricObj *cone, *base, *qsphere, *cylinder;

void grass()
{

    double topWidth = 0.6;
    double topThick = 0.02;
    double legThick = 0.02;
    double legLen = 0.3;
    float tam = 0.5;

    glPushMatrix();
        glTranslated( 0, legLen, 0);
        glScaled( topWidth, topThick, topWidth);
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
    float myTam= 0.3;
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
    float myTam= 0.3;
    float hTriangle = 0.5;  ///ALTURA PARA TRIangulo de techo
    glPushMatrix();
        glTranslated(0.4, 0.02, 0.4);

        //glPushMatrix();
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glTranslatef(0,0.25f,0);

            glBegin(GL_QUADS);
                // Front Face
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-myTam, -myTam,  myTam);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( myTam, -myTam,  myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam, myTam,  myTam);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-myTam,  myTam,  myTam);
                // Back Face
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-myTam, -myTam, -myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-myTam,  myTam, -myTam);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam);
                glTexCoord2f(0.0f, 0.0f); glVertex3f( myTam, -myTam, -myTam);
            glEnd();

            glBegin(GL_QUADS);
          // Top Face
                /*glTexCoord2f(0.0f, 1.0f); glVertex3f(-myTam,  myTam, -myTam);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-myTam,  myTam,  myTam);
                glTexCoord2f(1.0f, 0.0f); glVertex3f( myTam,  myTam,  myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam);*/
            // Right face
                glTexCoord2f(1.0f, 0.0f); glVertex3f( myTam, -myTam, -myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam);
                glTexCoord2f(0.0f, 1.0f); glVertex3f( myTam,  myTam,  myTam);
                glTexCoord2f(0.0f, 0.0f); glVertex3f( myTam, -myTam,  myTam);
                // Left Face
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-myTam, -myTam, -myTam);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-myTam, -myTam,  myTam);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(-myTam,  myTam,  myTam);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-myTam,  myTam, -myTam);
            glEnd();

            glBegin(GL_TRIANGLES);
                // Right triangle wall
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam);///1
                glTexCoord2f(0.5f, 0.5f); glVertex3f( myTam, hTriangle, 0.0f);///2R
                glTexCoord2f(0.0f, 1.0f); glVertex3f( myTam,  myTam,  myTam);///3
                // Left triangle wall
                glTexCoord2f(1.0f, 1.0f); glVertex3f( -myTam, myTam,  myTam);///A
                glTexCoord2f(0.5f, 0.5f); glVertex3f( -myTam, hTriangle ,0);///2L
                glTexCoord2f(0.0f, 1.0f); glVertex3f( -myTam,  myTam,  -myTam);///B
            glEnd();

            //Roof;
            glBindTexture(GL_TEXTURE_2D, texture[3]);
            glBegin(GL_QUADS);
                //Front roof
                glTexCoord2f(1.0f, 1.0f); glVertex3f( -myTam, myTam,  myTam);  ///A
                glTexCoord2f(0.0f, 1.0f); glVertex3f(  myTam, myTam,  myTam);  ///3
                glTexCoord2f(0.0f, 0.26f); glVertex3f(  myTam, hTriangle, 0.0f); ///2R
                glTexCoord2f(1.0f, 0.26f); glVertex3f( -myTam, hTriangle, 0.0f); ///2L
                //Back roof
                glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam,  -myTam);  ///1
                glTexCoord2f(0.0f, 1.0f); glVertex3f( -myTam,  myTam, -myTam);  ///B
                glTexCoord2f(0.0f, 0.26f); glVertex3f(-myTam, hTriangle ,0);     ///2L
                glTexCoord2f(1.0f, 0.26f); glVertex3f( myTam, hTriangle, 0.0f);  ///2R
            glEnd();

    glPopMatrix();

}
void tree()
{
    float myTam = 0.3;
    float myTam2 = 0.3;
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glPushMatrix();
        //glTranslated(0.4, 0.02, 0.4);
         glBegin(GL_QUADS);
            ///paralelo a right wall
            glTexCoord2f(0.0f, 0.0f); glVertex3f( myTam, -myTam, -myTam);///1
            glTexCoord2f(0.0f, 1.0f); glVertex3f( myTam,  myTam, -myTam);///2
            glTexCoord2f(1.0f, 1.0f); glVertex3f( myTam,  myTam,  myTam);///3
            glTexCoord2f(1.0f, 0.0f); glVertex3f( myTam, -myTam,  myTam);///4

        glEnd();
    glPopMatrix();

    glPushMatrix();
        glTranslated(myTam, 0, 0);
        ///paralelo a door
        glBegin(GL_QUADS);
            glTexCoord2f(1.0f, 0.0f); glVertex3f( -myTam2, -myTam2, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(  myTam2, -myTam2, 0);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(  myTam2,  myTam2, 0);
            glTexCoord2f(1.0f, 1.0f); glVertex3f( -myTam2,  myTam2, 0);
        glEnd();
    glPopMatrix();
    glPopMatrix();

}


float x, y, z;

void theWindow()
{
    float myp= 0.2;
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
    float myp= 0.2;
    glBindTexture(GL_TEXTURE_2D, texture[2]);
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


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

    glViewport(0, 0, WindowSizX,WindowSizY );


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

    /// Building Block

    glPushMatrix();
        //glBindTexture(GL_TEXTURE_2D, texture[2]);
        glTranslated(-1,0,-0.8);
        glScalef(1.5f, 1.0f,1.0f);
        building();
    glPopMatrix();

    ///Tree
    glPushMatrix();
        glTranslated(0.4, 0.25, 0.4);
        tree();
    glPopMatrix();


    /// Grass
    glPushMatrix();
        //glBindTexture(GL_TEXTURE_2D,texture[0]);
        glEnable(GL_TEXTURE_2D);
        glScalef(4,1,4);
        glTranslated(0,-0.33,0);
        grass();
        //glDisable(GL_TEXTURE_2D);
    glPopMatrix();


    /// window down

    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
    //doorFinal();
        glScalef(0.5f,0.5f,0.5f);
        glTranslatef(-0.3f,0.65f,-0.39f);
        theWindow();
    glPopMatrix();

    /// door final

    glPushMatrix();
        glScalef(0.6f,1.2f,0.5f);
        glTranslatef(-0.8f,0.175f,-0.399f);
        //glScalef(0.5f,0.6f,0.5f);
        //twofinalWindow();
        theDoor();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

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

    /*printf("EyePosition_X: %f\n",EyePosition_X);
    printf("EyePosition_Y: %f\n",EyePosition_Y);
    printf("EyePosition_Z: %f\n\n",EyePosition_Z);*/
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
    else if(key == 27)  exit(0);

}


char* filename[] = {"C:/Users/USUARIO/Documents/2018/02/Grafica/casawee2/Grass01.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/casaWe/wall.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/casaWee2/door.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/casawee2/roof2.bmp",
                    "C:/Users/USUARIO/Documents/2018/02/Grafica/casawee2/grass.bmp"};


int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutInitWindowSize(WindowSizX, WindowSizY);
    glutCreateWindow("3D Home");
    glutKeyboardFunc(KeybordFunction);

    glutIdleFunc(RepeatDisplay);

    for ( int  i=0;i<5;i++)
   {
      loadTextureFromFile(filename[i],i);
   }

    glutDisplayFunc(display);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_NORMALIZE);

    glutSpecialFunc(specialKeys);
    glutFullScreen();

    //Init();

    glutMainLoop();

    return 0;
}
