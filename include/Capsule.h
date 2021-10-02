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
    int lats;
    int longs;
    Point axisDirection;
    float* openGLVertices;
    float* openGLNormals;
    int openGLArrayLength;
public:
    Point getAxisDirection();
    float getRadius();
    float getLength();
    Point getCylinderPositiveEnd();
    Point getCylinderNegativeEnd();
    Capsule(string, bool, Point, Point, Point, Point, Point, float, float, Color, float, float, int, int);
    Capsule(string, Color, Point*, Matrix*, int, float, float, int, int);
    float getMinX();
    float getMinY();
    float getMinZ();
    float getMaxX();
    float getMaxY();
    float getMaxZ();
    float getLats();
    float getLongs();
    void setRotation(Matrix);
};

#endif
