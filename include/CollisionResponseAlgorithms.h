#ifndef CRALG_H
#define CRALG_H

#include "Object.h"
#include "Point.h"
#include "Collision.h"
#include "Ball.h"
#include "Capsule.h"
#include "Plane.h"
#include <map>

using namespace std;

void moveObjects(Object**, int, float, bool);
void calculateNonStaticCollision(Object*, Object*, Point, Point);
bool handleContact(Object*, Object*, Point, Point, Point);
void calculateStaticCollision(Object*, Object*, Point, Point);
void collisionResponse(map<string, Collision>);

#endif
