#ifndef SCENE_H
#define SCENE_H

#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <vector>

using namespace std;

class Stone
{
public:
    float cx;
    float cy;
    float cz;
    float sz;
    float mx, my, mz;

public:
    Stone(float _cx, float _cy, float _cz, float _sz): cx(_cx), cy(_cy), cz(_cz), sz(_sz){}
    void draw();
    vector<float> getPos();
    float getSize();

};

class Many_Stones
{
public:
    vector<Stone> vec_stones;
    float minz = 0.0f;
    float lastz;
    float square_size;
public:
    Many_Stones(float _square_size);
    void draw(float doll_posz);
    void addStone(float cx, float cy, float cz, float sz);
    vector<Stone> getAllStones();
};


class scene
{
    public:
        float trackOffset;
        int iSecret;
        int iSecret2;
        float largoQuad;
        float square_size = 10.0f;
        //Many_Stones mStones;
        vector<Stone> vec_stones;
        float max_roads = 50;
        scene();
        //void detectarColsiones();
        void drawQuad(int posTexture=2);
        void createLine(int);
        void drawScene(float ,bool);

    protected:

    private:
};

#endif // SCENE_H
