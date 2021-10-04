#ifndef CAPSULE_H
#define CAPSULE_H
#define _USE_MATH_DEFINES

#include "SDL_opengl.h"
#include "Object.h"

using namespace std;

class Capsule : public Object {
private:
    float radius;
    float length;
    bool drawRocket;
    Point axisDirection;
public:
    Point getAxisDirection();
    float getRadius();
    float getLength();
    Point getCylinderPositiveEnd();
    Point getCylinderNegativeEnd();
    Capsule(string, bool, Point, Point, Point, Point, Point, float, float, Color, bool, float, float);
    Capsule(string, Color, Point*, Matrix*, int, bool, float, float);
    float getMinX();
    float getMinY();
    float getMinZ();
    float getMaxX();
    float getMaxY();
    float getMaxZ();
    bool getDrawRocket();
    void setRotation(Matrix);
};

#endif
