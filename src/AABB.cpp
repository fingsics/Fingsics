#include "../include/AABB.h"

AABBPoint::AABBPoint(Object* object, double value, bool isMin) {
    this->object = object;
    this->value = value;
    this->isMin = isMin;
}

AABBPoint::AABBPoint() {
    this->object = NULL;
    this->value = 0;
    this->isMin = false;
}

AABB::AABB(Object* object) {
    this->object = object;
    this->minX = AABBPoint(object, INF, true);
    this->minY = AABBPoint(object, INF, true);
    this->minZ = AABBPoint(object, INF, true);
    this->maxX = AABBPoint(object, INF, false);
    this->maxY = AABBPoint(object, INF, false);
    this->maxZ = AABBPoint(object, INF, false);
}

AABB::AABB() {
    this->object = NULL;
}
