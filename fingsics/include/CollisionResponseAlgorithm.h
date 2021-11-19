#ifndef CRALG_H
#define CRALG_H

#include "Object.h"
#include "Vector.h"
#include "Collision.h"
#include <map>

using namespace std;

class CollisionResponseAlgorithm {
private:
    static void calculateNonStaticCollision(Object*, Object*, Vector, Vector);
    static void calculateStaticCollision(Object*, Object*, Vector, Vector);
public:
    static void collisionResponse(map<string, Collision>);
    static void moveObjects(vector<Object*>, float, bool);
};


#endif
