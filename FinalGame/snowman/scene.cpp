#include "scene.h"

extern GLuint texture[3];         // Storage For One Texture ( NEW )

float aleatoriFloat()
{
    double i = 0, d = 0;
    i = rand() % 40 - 20; //Gives a number between -20 and +20;
    d = i / 100; //Reduces this number to the range you want.
    return d;
}
/* ------------ stone ------------------- */

void Stone::draw()
{
    glPushMatrix();
        glTranslatef(cx, 1.0f, cz);
        glutSolidSphere(1, 20, 20);   /// 1 es el tamano de la rocka
    glPopMatrix();
}

vector<float> Stone::getPos()
{
    vector<float> vecPos;
    vecPos.push_back(cx);
    vecPos.push_back(cy);
    vecPos.push_back(cz);
    return vecPos;
}

float Stone::getSize()
{
    return sz;
}

/* --------- STONES ------------------- */

Many_Stones::Many_Stones(float _square_size): square_size(_square_size)
{
    lastz = 0;
    for(int i = 0;i < 4;++i)   /// cantidad de piedras
    {
        if(rand()%100 < 10){  //10% probabilidad de que aparesca el camino
            addStone(0.0f, 0.0f, (aleatoriFloat()*2

                                  ) , 2.5f);
        }
        lastz -= square_size;
    }
}

void Many_Stones::draw(float doll_posz)
{
    minz = doll_posz;
    for(Stone& s: vec_stones)
    {
        if(s.cz > minz) // Si se oculta la piedra de la vista, recalcularla
        {
            s.cz = lastz;
            lastz -= doll_posz;
//            cout << "Recalcular" << endl;
//            Sleep(100);
        }
        s.draw();
    }
}

void Many_Stones::addStone(float cx, float cy, float cz, float sz)
{
    vec_stones.emplace_back(cx, cy, cz, sz);
}

vector<Stone> Many_Stones::getAllStones()
{
    return this->vec_stones;
}

scene::scene():  mStones(square_size)
{
    //square_size = 10.0f;
    trackOffset = 0;
    srand (time(NULL));
    iSecret = 0;
    iSecret2 = 0;
    largoQuad = -20.0f;
    //ctor
}

void scene::drawQuad(int posTexture){
    glBegin(GL_QUADS);
        glBindTexture(GL_TEXTURE_2D, texture[posTexture]);
        glTexCoord2f(0.0, 0.0); glVertex3f(-0.7f, 0.0,  10.0f );
        glTexCoord2f(1.0, 0.0); glVertex3f( 0.7f, 0.0,  10.0f );
        glTexCoord2f(1.0, 10.0); glVertex3f( 0.7f, 0.0, largoQuad);
        glTexCoord2f(0.0, 10.0); glVertex3f(-0.7f, 0.0, largoQuad);
    glEnd();
}

void scene::detectarColsiones()
{

}

void scene::createLine(int aleatori)
{
    if(aleatori == 1){
        //No dibujar
        glTranslatef(-2.5, 0.0, 0.0 );
    }else {
        ///pista 1
        glTranslatef(-2.5, 0.0, 0.0 );
        drawQuad(2);
        mStones.draw( (float(rand() % 10 + 1),0, float(rand() % 10 + 1) ) );
    }
    if(aleatori == 2){
         //No dibujar
        glTranslatef(2.5, 0.0, 0.0 );    ///mover en X
    }else{
        /// pista 2
        glTranslatef(2.5, 0.0, 0.0 );    ///mover en X
        drawQuad(2);
        mStones.draw( (float(rand() % 20 + 1),float(rand() % 4 + 1), float(rand() % 10 + 1) ) );
    } if(aleatori == 3){
         //No dibujar
        glTranslatef(2.5, 0.0, 0.0 );   /// mover en
    }else{
        ///pista 3
        glTranslatef(2.5, 0.0, 0.0 );   /// mover en X
        drawQuad(2);
        mStones.draw( (float(rand() % 20 + 1),float(rand() % 4 + 1), float(rand() % 10 + 1) ) );
    }
}

void scene::drawScene(float offset, bool gameStarted)
{

	if(gameStarted){
		trackOffset += 0.1;///0.4;
        //iSecret = rand() % 10 + 1;
        //if(iSecret == 1 ) cout<<"iSe " <<iSecret<<endl;
	}
	if( trackOffset>(largoQuad*(-1)+15) ){
		trackOffset=0.0;
        iSecret = rand() % 10 + 1;
        iSecret2 = rand() % 4 + 1;
	}

	glPushMatrix();
        glDisable(GL_LIGHTING);
        glTranslatef(0.0, 0.0, trackOffset);
        glTranslatef(offset, 0.0, 0.0);

        ///Pista 1 front
        createLine(iSecret);

        ///pista 2 back
        glTranslatef(-2.5, 0, largoQuad-15 );
        createLine(iSecret2);

	glEnable(GL_LIGHTING);
	glPopMatrix();
	//glDisable(GL_TEXTURE_2D);

}
