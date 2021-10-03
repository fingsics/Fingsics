#ifndef BALL_H
#define BALL_H
#define _USE_MATH_DEFINES

#include "SDL_opengl.h"
#include "Object.h"

using namespace std;

class Ball : public Object {
    private:
        float radius;
    public:
        Ball(string, bool, Point, Point, Point, Point, Point, float, float, Color, bool, float);
        Ball(string, Color, Point*, Matrix*, int, bool, float);
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
