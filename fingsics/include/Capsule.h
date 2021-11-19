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
    Vector axisDirection;
public:
    Vector getAxisDirection();
    float getRadius();
    float getLength();
    Vector getCylinderPositiveEnd();
    Vector getCylinderNegativeEnd();
    Capsule(string, bool, Vector, Vector, Vector, Vector, Vector, float, float, Color, bool, bool, float, float);
    Capsule(string, Color, Vector*, Matrix*, int, bool, bool, float, float);
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
