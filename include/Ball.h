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

using namespace std;

class Ball : public Object {
    private:
        float radius;
        int lats;
        int longs;
    public:
        Ball(string, bool, Point, Point, Point, Point, Point, float, float, Color, float, int, int);
        void drawObject();
        float getRadius();
        Matrix getInertiaTensorInverse();
        float getMinX();
        float getMinY();
        float getMinZ();
        float getMaxX();
        float getMaxY();
        float getMaxZ();
};

#endif
