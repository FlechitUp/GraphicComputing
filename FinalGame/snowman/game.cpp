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
