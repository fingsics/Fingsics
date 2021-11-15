#ifndef AABB_H
#define AABB_H

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

class Object;

struct AABB;

struct AABBPoint {
    float value;
    bool isMin;
    AABB* aabb;
    AABBPoint();
    AABBPoint(AABBPoint*);
};

struct AABB {
    AABBPoint* minX;
    AABBPoint* maxX;
    AABBPoint* minY;
    AABBPoint* maxY;
    AABBPoint* minZ;
    AABBPoint* maxZ;
    Object* object;
    AABB();
    AABB(Object*);
};


#include "Object.h"
#endif