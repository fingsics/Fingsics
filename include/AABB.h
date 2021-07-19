#ifndef AABB_H
#define AABB_H

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

class Object;

struct AABBPoint {
    Object* object;
    double value;
    bool isMin;
    int index;
    AABBPoint(Object*, double, bool);
    AABBPoint();
};

struct AABB {
    AABBPoint minX;
    AABBPoint minY;
    AABBPoint minZ;
    AABBPoint maxX;
    AABBPoint maxY;
    AABBPoint maxZ;
    Object* object;
    AABB();
    AABB(Object*);
};


#include "Object.h"
#endif