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
    //Point torque;????
public:
    Point getAxisDirection();
    double getRadius();
    double getLength();
    Point getCylinderEnd1();
    Point getCylinderEnd2();
    Capsule(string, Point, Point, Point, double, double, Point, Point, double, double, Color);
    void draw();
    void updatePosAndVel(double);
    Matrix getInertiaTensor();
};

#endif
