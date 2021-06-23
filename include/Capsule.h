#ifndef CAPSULE_H
#define CAPSULE_H
#include "SDL_opengl.h"
#include "Color.h"
#include "Point.h"
#include "Object.h"
#include <vector>
#include <math.h>
#include <string>

#define M_PI 3.1415926
#define LATS 8
#define LONGS 8

using namespace std;

class Capsule : public Object {
private:
    double radius;
    double length;
    // Pitch, yaw, roll
    Point angle;
    Point angularVelocity;
    //Point torque;????
public:
    Capsule(string, Point, Point, Point, double, double, Point, Point, double, double, Color);
    void draw();
    void updatePosAndVel(double, Room);
};

#endif
