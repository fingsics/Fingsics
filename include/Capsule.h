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
    double radius;
    double length;
    int lats;
    int longs;
public:
    Point getAxisDirection();
    double getRadius();
    double getLength();
    Point getCylinderPositiveEnd();
    Point getCylinderNegativeEnd();
    Capsule(string, bool, Point, Point, Point, Point, Point, double, double, Color, double, double, int, int);
    void drawObject();
    double* getMins();
    double* getMaxes();
    double getMinX();
    double getMinY();
    double getMinZ();
    double getMaxX();
    double getMaxY();
    double getMaxZ();
};

#endif
