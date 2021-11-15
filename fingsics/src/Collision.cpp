#include "../include/Collision.h"

using namespace std;

Collision::Collision(Point point, Point normal, float penetrationDepth) {
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

float Collision::getPenetrationDepth() {
    return penetrationDepth;
}

float Collision::getLastPenetrationDepth() {
    return lastPenetrationDepth;
}

void Collision::invertNormal() {
    normal = normal * -1;
}

void Collision::setObjects(pair<Object*, Object*> objects) {
    this->object1 = objects.first;
    this->object2 = objects.second;
}

void Collision::setLastPenetrationDepth(float lastPenetrationDepth) {
    this->lastPenetrationDepth = lastPenetrationDepth;
}

void Collision::setSecondToLastPenetrationDepth(float secondToLastPenetrationDepth) {
    this->secondToLastPenetrationDepth = secondToLastPenetrationDepth;
}

bool Collision::shouldApplyIt() {
    return lastPenetrationDepth == -1 || penetrationDepth > lastPenetrationDepth;
}