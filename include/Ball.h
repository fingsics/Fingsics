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
#define LATS 15
#define LONGS 15

using namespace std;

class Ball : public Object {
    private:
        double rad;

    public:
        Ball(string, Point, Point, double, double, Color);
        void draw();
        double getRad();
};

#endif
