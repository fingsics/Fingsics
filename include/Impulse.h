#ifndef IMPULSE_H
#define IMPULSE_H

#include "Point.h"

using namespace std;

struct Impulse {
    Point normal;
    Point tangent;
    double magnitude;
    double mass;
    Impulse(Point, Point, double, double);
};

#endif
