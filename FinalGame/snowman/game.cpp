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
		myDoll.drawCharacter();//drawCharacter(rotAngle, jumpOffset, proneAngle, shiftAngle);
        //detectarColsiones();
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
    float dx = myDoll.position, dy = myDoll.jumpOffset, dz =0;
    for(Stone& st: myscene.mStones.getAllStones())
    {
        vector<float> stpos = st.getPos();
        float sx = stpos[0], sy = stpos[1], sz = stpos[2];
        float to_colision = 0.77f + st.getSize();
        float distance_to_stone = sqrt(pow(sx-dx, 2.0f) + pow(sy-dy, 2.0f) + pow(sz-dz, 2.0f) );
        if(distance_to_stone < to_colision)
        {
            cout << "COLISIONA CON ROCA" << distance_to_stone << endl;
            //mostrarCirculo = true;
            break;
        }
        //else
            //mostrarCirculo = false;

    }
}

