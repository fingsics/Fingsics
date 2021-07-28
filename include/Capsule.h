#ifndef CAPSULE_H
#define CAPSULE_H
#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>
#include <string>
#include "SDL_opengl.h"
#include "Color.h"
#include "Point.h"
#include "Object.h"

using namespace std;

class Capsule : public Object {
private:
    float radius;
    float length;
    int lats;
    int longs;
    Point axisDirection;
public:
    Point getAxisDirection();
    float getRadius();
    float getLength();
    Point getCylinderPositiveEnd();
    Point getCylinderNegativeEnd();
    Capsule(string, bool, Point, Point, Point, Point, Point, float, float, Color, float, float, int, int);
    void drawObject();
    float getMinX();
    float getMinY();
    float getMinZ();
    float getMaxX();
    float getMaxY();
    float getMaxZ();
    void setRotation(Matrix);
};

#endif
