#ifndef ROOM_H
#define ROOM_H

#include "SDL_opengl.h"

using namespace std;

class Room {
    private:
        float floor;
        float leftWall;
        float rightWall;
        float backWall;
        float frontWall;
        float height;
        bool drawFrontWall;
        bool drawBackWall;
        bool drawLeftWall;
        bool drawRightWall;
    public:
        Room();
        Room(float, float, float, float, float, float);
        float getFloor();
        float getLeftWall();
        float getRightWall();
        float getFrontWall();
        float getBackWall();
        void draw();
};

#endif