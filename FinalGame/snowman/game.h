#ifndef GAME_H
#define GAME_H

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <windows.h>
#include <iostream>
#include "scene.h"
#include "Doll.h"

//extern bool gameStarted;
//extern GLuint texture[3];         // Storage For One Texture ( NEW )

class gameController
{
public:
    void startRun();

    float trackOffset;/// 68
    Doll myDoll;
    scene myscene;
    gameController();//: myDoll(),myscene(){}
    bool gameStarted;

    void startGame();
    //void timer(int value);
    void actions(char);
};


#endif // GAME_H
