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

#define LATS 8
#define LONGS 8

using namespace std;

class Capsule : public Object {
private:
    double radius;
    double length;
public:
    Point getAxisDirection();
    double getRadius();
    double getLength();
    Point getCylinderPositiveEnd();
    Point getCylinderNegativeEnd();
    Capsule(string, Point, Point, Point, Point, Point, double, double, Color, double, double);
    void draw();
};

#endif
