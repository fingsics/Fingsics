#include "../include/AABB.h"

AABBPoint::AABBPoint() {
    this->aabb = NULL;
    this->value = INF;
    this->isMin = false;
}

AABBPoint::AABBPoint(AABBPoint* point) {
    this->aabb = point->aabb;
    this->value = point->value;
    this->isMin = point->isMin;
}

AABB::AABB(Object* object) {
    this->object = object;
    this->minX = NULL;
    this->minY = NULL;
    this->minZ = NULL;
    this->maxX = NULL;
    this->maxY = NULL;
    this->maxZ = NULL;
}

AABB::AABB() {
    this->object = NULL;
    this->minX = NULL;
    this->minY = NULL;
    this->minZ = NULL;
    this->maxX = NULL;
    this->maxY = NULL;
    this->maxZ = NULL;
}
