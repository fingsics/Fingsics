#ifndef IMPULSE_H
#define IMPULSE_H

#include "Point.h"

using namespace std;

struct Impulse {
    Point normal;
    Point tangent;
    float magnitude;
    float mass;
    Impulse(Point, Point, float, float);
};

#endif
