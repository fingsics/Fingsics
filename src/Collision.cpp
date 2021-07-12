#include "../include/Collision.h"

using namespace std;

Collision::Collision(Object* object1, Object* object2, Point point, Point normal, double penetrationDepth, double lastPenetrationDepth) {
    this->object1 = object1;
    this->object2 = object2;
    this->point = point;
    this->normal = normal;
    this->penetrationDepth = penetrationDepth;
    this->lastPenetrationDepth = lastPenetrationDepth;
}

Collision::Collision(Point point, Point normal, double penetrationDepth) {
    this->object1 = NULL;
    this->object2 = NULL;
    this->point = point;
    this->normal = normal;
    this->penetrationDepth = penetrationDepth;
    this->lastPenetrationDepth = -1;
}

Object* Collision::getObject1() {
    return object1;
}

Object* Collision::getObject2() {
    return object2;
}

Point Collision::getPoint() {
    return point;
}

Point Collision::getNormal() {
    return normal;
}

double Collision::getPenetrationDepth() {
    return penetrationDepth;
}

double Collision::getLastPenetrationDepth() {
    return lastPenetrationDepth;
}

void Collision::invertNormal() {
    normal = normal * -1;
}

void Collision::setObjects(Object* object1, Object* object2) {
    this->object1 = object1;
    this->object2 = object2;
}

void Collision::setLastPenetrationDepth(double lastPenetrationDepth) {
    this->lastPenetrationDepth = lastPenetrationDepth;
}