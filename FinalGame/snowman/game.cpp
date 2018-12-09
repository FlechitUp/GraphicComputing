#include "game.h"

using namespace std;

gameController::gameController() ///as startGame
{
    //cout<<"oko";
    gameStarted = false;
    //initCamera();
}
void gameController::startGame()
{
	gameStarted = true ; 		/// 33
}

void gameController::startRun()  /// del game controller
{
	// vacio
    myscene.drawScene(-myDoll.characterOffset, gameStarted);
    if(gameStarted)
	{
		myDoll.initCharacter();
		myDoll.drawCharacter();//drawCharacter(rotAngle, jumpOffset, proneAngle, shiftAngle)
		if (myDoll.lifes ==  0) gameStarted =false;
        detectarColsiones();
	}
}

void gameController::actions(char opt)
{
    if (opt == 'w')
        myDoll.jump();
    else if(opt == 's')
        myDoll.down();
    else if(opt == 'a')
        myDoll.left();
    else if(opt == 'd')
        myDoll.right();
}

void gameController::detectarColsiones()
{
    //float dx = myDoll.position, dy = myDoll.jumpOffset, dz = 0;
    float dx = myDoll.getPos()[0]+myDoll.position, dy = myDoll.getPos()[1], dz = myDoll.getPos()[2];

    for(Stone& st: myscene.vec_stones )
    {
        vector<float> stpos = st.getPos();
        float sx = stpos[0], sy = stpos[1], sz = stpos[2]-myscene.trackOffset;
        //cout<< sx<<" " <<sy <<endl;
        float to_colision = 0.77f + st.getSize();
        float distance_to_stone = sqrt(pow(sx-dx, 2.0f) + pow(sy-dy, 2.0f) + pow(sz-dz, 2.0f) );
        cout << sx << " " << sy << " " << sz << endl;
        if(distance_to_stone < to_colision)
        {
//            cout << "Roooooooooooooocas" << distance_to_stone << endl;
            //mostrarCirculo = true;
            myDoll.lifes--;
            break;
        }
        //else
            //mostrarCirculo = false;

    }
}


