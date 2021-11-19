#ifndef IMPULSE_H
#define IMPULSE_H

#include "Vector.h"

using namespace std;

struct Impulse {
    Vector normal;
    Vector tangent;
    float magnitude;
    float mass;
    Impulse(Vector, Vector, float, float);
};

#endif
