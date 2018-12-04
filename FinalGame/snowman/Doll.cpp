#include "Doll.h"

using namespace std;

Doll::Doll()
{
    rotAngle = 0; ///angulo de rotacion
    jumpOffset = 0;
    proneAngle = 0;
    shiftAngle = 0;
    position = 0 ;
    characterOffset = 0;
	jumping = false;
	proning = true;//false;
	proneHold = 0;
	handAngle = 110;
    handDir = 1;
}

void Doll::detectarColisiones()
{

}

void Doll::drawCharacter()
{
    glDisable(GL_LIGHTING);
	if(handDir==1 &&handAngle >50)
		handAngle-=3;
	else if(handDir==-1 && handAngle <110)
		handAngle+=3;
	if(handDir==1 &&  handAngle<=50)
		handDir=-1;
	else if(handDir==1 && handAngle>=110)
		handDir = 1;

	glPushMatrix();
        glTranslatef(0.0, jumpOffset, -2.0);
        glRotatef(rotAngle, 0.0, 1.0, 0.0);
        glRotated(shiftAngle, 0.0, 1.0, 1.0);
        /// body
        glColor3f(0.0f, 0.5f, 1.0f);
        glTranslatef(0.0f, 0.75f, 0.0f);
        glRotatef(-proneAngle, 1.0, 0.0, 0.0);
        glutSolidSphere(0.77f,20, 20);

        ///Arms
        glPushMatrix();
            ///cilindro izq
            glColor3f(0.0, 1.0 , 1.0);
            glTranslatef(0.59, 0.4, 0.0);
            glRotatef(handAngle, 0.0, 1.0 , 0.0);
            GLUquadricObj* quadratic;		//34
            quadratic = gluNewQuadric();
            gluCylinder(quadratic, 0.1, 0.1, 0.4, 20,20 );

            glColor3f(0.0f, 1.0f, 0.0f);
            glTranslatef(0.0, 0.0, 0.5);
            glutSolidSphere(0.15f, 10, 10);
            glTranslatef(0.0, 0.0, -0.5);

            ///cilindro derecho
            glColor3f(0.0, 1.0, 1.0);
            glRotatef(-handAngle, 0.0, 1.0, 0.0);
            glTranslatef(-1.18, 0.0, 0.0);
            glRotatef(-handAngle, 0.0, 1.0, 0.0);
            gluCylinder(quadratic, 0.1, 0.1, 0.4, 20, 20);

            glColor3f(1.0, 0.0, 0.0);
            glTranslatef(0.0, 0.0, 0.5);
            glutSolidSphere(0.15,10,10);
        glPopMatrix();		/// 50

        glTranslatef(0.0f, 1.0f, 0.0f);
        glutSolidSphere(0.25f, 20, 20);		// 53

        /// head
        glPushMatrix();
            glColor3f(0.0f, 0.0,0.0);
            glTranslatef(0.05f, 0.10f, 0.18f );
            glutSolidSphere(0.05f, 10, 10);
            glTranslatef(-0.1f, 0.0f, 0.0f);
            glutSolidSphere(0.05f , 10, 10);
        glPopMatrix();			/// 61

        ///Nose
        glColor3f(1.0f, 0.5f, 0.5f);
        glutSolidCone(0.08f, 0.5f,10, 2);
        glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();			// 66
	glEnable(GL_LIGHTING);
    //std::cout<<texture[0]<<std::endl;
}

vector<float> Doll::getPos()
{
    vector<float> vecPos;
/*    vecPos.push_back(cx);
    vecPos.push_back(cy);
    vecPos.push_back(cz);*/
    return vecPos;
}


void Doll::initCharacter()  		/// 55
{
	if(rotAngle<180)
		rotAngle+=2;
	//if(rotAngle==180)
		//startRun();
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
void Doll::jump()
{
    if(jumpOffset==0.0)
	{
        jumping = true;
		if(proning)
            proning = false;
    }
}

void Doll::down()
{
    if(proneAngle==0.0)
        proning = true;
    else if(jumpOffset!=0)
		proning = true;
}

void Doll::right()  /// press d
{
    shiftAngle =0.0;
    shifting = 1;
	position++;
	if(position>=-1)
        position=1;
}
void Doll::left()   /// press 'a'
{
    shiftAngle =0.0;
    shifting =1;
	position--;
	if(position<-1)
        position=-1;
}
