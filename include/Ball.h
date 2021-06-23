#ifndef BALL_H
#define BALL_H
#include "SDL_opengl.h"
#include "Color.h"
#include "Point.h"
#include "Object.h"
#include <vector>
#include <math.h>
#include <string>

#define M_PI 3.1415926
#define LATS 8
#define LONGS 8

using namespace std;

class Ball : public Object {
    private:
        double rad;

    public:
        Ball(string, Point, Point, Point, double, double, double, Color);
        void draw();
        double getRad();
        void updatePosAndVel(double, Room);
};

#endif
