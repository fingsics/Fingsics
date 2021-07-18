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
    this->minX = AABBPoint(object, object->getMinX(), true);
    this->minY = AABBPoint(object, object->getMinY(), true);
    this->minZ = AABBPoint(object, object->getMinZ(), true);
    this->maxX = AABBPoint(object, object->getMaxX(), true);
    this->maxY = AABBPoint(object, object->getMaxY(), true);
    this->maxZ = AABBPoint(object, object->getMaxZ(), true);
}

AABB::AABB() {
    this->object = NULL;
}
