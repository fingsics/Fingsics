#ifndef CRALG_H
#define CRALG_H

#include "Object.h"
#include "Point.h"
#include "Collision.h"
#include "Ball.h"
#include "Capsule.h"
#include <map>

using namespace std;

class CollisionResponseAlgorithm {
private:
    static bool handleContact(Object*, Object*, Point, Point, Point);
    static void calculateNonStaticCollision(Object*, Object*, Point, Point);
    static void calculateStaticCollision(Object*, Object*, Point, Point);
public:
    static void collisionResponse(map<string, Collision>);
    static void moveObjects(Object**, int, float, bool);
};


#endif
