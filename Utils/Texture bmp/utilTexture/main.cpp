#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"
GLfloat xRotated, yRotated, zRotated;
GLuint  texture[3];         // Storage For One Texture ( NEW )
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

/*
* Draw the texture in the OpenGL graphics window
*/
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);

   glLoadIdentity();
   glTranslatef(0.0,0.0,-5);
     glRotatef(yRotated, 0, 1, 0);
     glRotatef(zRotated, 0, 0, 1);

  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glBegin(GL_QUADS);
        // Front Face

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(2.0f, 2.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 2.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        // Back Face
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
  glEnd();
   glPopMatrix();

  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_QUADS);
        // Top Face
        //glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        // Bottom Face

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        // Right face
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
  glEnd();
      glPopMatrix();

  glPushMatrix();
  glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
        // Left Face
        //glBindTexture(GL_TEXTURE_2D, texture[2]);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
  glPopMatrix();


   glFlush();
   glDisable(GL_TEXTURE_2D);

}

void resizeWindow(int x, int y)
{
 if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.5
    //Far clipping plane distance: 20.0

    gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);  //Use the whole window for rendering
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

char* filename[] = {"C:/Users/USUARIO/Documents/2018/02/Grafica/utilTexture/heart.bmp","./win030.bmp","./win044.bmp"};

void idleFunc(void)
{

     yRotated += 0.01;
     zRotated += 0.01;
    drawScene();
}
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(240, 240);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   for ( int  i=0;i<3;i++)
   {
      loadTextureFromFile(filename[i],i);
   }

   glutDisplayFunc(drawScene);
   glutReshapeFunc(resizeWindow);
   glutKeyboardFunc(keyboard);
     glutIdleFunc(idleFunc);
   glutMainLoop();
   return 0;
}
