#ifndef BALL_H
#define BALL_H
#include "SDL/SDL_opengl.h"
#include "Color.h"
#include "Point.h"
#include <vector>
#include <math.h>

#define M_PI 3.1415926
#define LATS 15
#define LONGS 15

using namespace std;

class Ball {
    private:
        double posX;
        double posY;
        double posZ;
        double velX;
        double velY;
        double velZ;
        double rad;
        double mass;
        Color* color;
        void decreaseVelocity(double);

    public:
        Ball(double, double, double, double, double, Color*);
        void setVelocity(Point*);
        void draw();
        void updatePosAndVel(double, Ball**);
        double getPosX();
        double getPosY();
        double getPosZ();
        double getRad();
        double getMass();
        bool isMoving();
        Point* getPos();
        Point* getVel();
        void setPos(Point*);
};

#endif
