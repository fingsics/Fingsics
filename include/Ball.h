#ifndef BALL_H
#define BALL_H
#define _USE_MATH_DEFINES
#include "SDL_opengl.h"
#include "Color.h"
#include "Point.h"
#include "Object.h"
#include <vector>
#include <math.h>
#include <string>

#define LATS 8
#define LONGS 8

using namespace std;

class Ball : public Object {
    private:
        double rad;

    public:
        Ball(string, Point, Point, Point, Point, Point, double, double, double, Color);
        void draw();
        double getRad();
        void updatePosAndVel(double);
        Matrix getInertiaTensor();
};

#endif
