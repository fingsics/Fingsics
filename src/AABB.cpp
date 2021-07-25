#include "../include/AABB.h"

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
