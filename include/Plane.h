#ifndef PLANE_H
#define PLANE_H
#define _USE_MATH_DEFINES
#include "SDL_opengl.h"
#include "Color.h"
#include "Point.h"
#include "Object.h"
#include <vector>
#include <math.h>
#include <string>

using namespace std;

class Plane : public Object {
    private:
        float drawLength;
        float drawWidth;
    public:
        Plane(string, bool, Point, Point, Point, Point, Point, float, float, Color, float, float);
        void drawObject();
        Point getNormal();
        Matrix getInertiaTensor();
        void drawOBB();
        void drawAABB();
        float getMinX();
        float getMinY();
        float getMinZ();
        float getMaxX();
        float getMaxY();
        float getMaxZ();
};

#endif
