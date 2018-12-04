#ifndef DOLL_H
#define DOLL_H

#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;

class Doll
{
    public:
        float rotAngle;
        float jumpOffset;
        float proneAngle;
        float shiftAngle;
        float characterOffset;
        int position;
        bool jumping;
        bool proning;
        float handAngle;
        short handDir;
        int proneHold;
        int shifting;

        Doll();
        vector<float> getPos();
        void detectarColisiones();
        void jump();
        void down();
        void right();
        void left();
        void drawCharacter();
        void initCharacter();

    protected:

    private:
};

#endif // DOLL_H
