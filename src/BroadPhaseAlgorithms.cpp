#include "../include/BroadPhaseAlgorithms.h"

using namespace std;

// Adds every posible object pair to the map
map<string, pair<Object*, Object*>> NoBroadPhase::getCollisions(Object** objects, int numObjects) {
    map<string, pair<Object*, Object*>> collisionMap;
    for (int i = 0; i < numObjects; i++) {
        for (int j = i + 1; j < numObjects; j++) {
            if (objects[i]->getIsStatic() && objects[j]->getIsStatic()) continue;
            pair<Object*, Object*> objectPair = make_pair(objects[i], objects[j]);
            string objectPairId = getObjectPairId(objectPair);
            if (collisionMap.find(objectPairId) == collisionMap.end()) {
                collisionMap.insert(pair<string, pair<Object*, Object*>>(objectPairId, objectPair));
            }
        }
    }
    return collisionMap;
}

bool BruteForceBroadPhase::AABBOverlapTest(Object* object1, Object* object2) {
    if (object1->getMinX() > object2->getMaxX() || object2->getMinX() > object1->getMaxX()) return false;
    if (object1->getMinY() > object2->getMaxY() || object2->getMinY() > object1->getMaxY()) return false;
    if (object1->getMinZ() > object2->getMaxZ() || object2->getMinZ() > object1->getMaxZ()) return false;
    return true;
}

map<string, pair<Object*, Object*>> BruteForceBroadPhase::getCollisions(Object** objects, int numObjects) {
    map<string, pair<Object*, Object*>> collisionMap;
    for (int i = 0; i < numObjects; i++) {
        for (int j = i + 1; j < numObjects; j++) {
            if (objects[i]->getIsStatic() && objects[j]->getIsStatic()) continue;
            pair<Object*, Object*> objectPair = make_pair(objects[i], objects[j]);
            string objectPairId = getObjectPairId(objectPair);
            if (collisionMap.find(objectPairId) == collisionMap.end() && (dynamic_cast<Plane*>(objects[i]) || dynamic_cast<Plane*>(objects[j]) || AABBOverlapTest(objects[i], objects[j]))) {
                collisionMap.insert(pair<string, pair<Object*, Object*>>(objectPairId, objectPair));
            }
        }
    }
    return collisionMap;
}

void SweepAndPruneBroadPhase::insertAABBPoints(Object* object, int insertedCount) {
    // TODO
}

SweepAndPruneBroadPhase::SweepAndPruneBroadPhase(Object** objects, int numObjects) {
    this->pointsPerAxis = 2 * numObjects;
    this->xPoints = new AABBPoint[2 * pointsPerAxis];
    this->yPoints = new AABBPoint[2 * pointsPerAxis];
    this->zPoints = new AABBPoint[2 * pointsPerAxis];
    for (int i = 0; i < numObjects; i++) insertAABBPoints(objects[i], i);
}

map<string, pair<Object*, Object*>> SweepAndPruneBroadPhase::getCollisions(Object** objects, int numObjects) {
    return collisionPairs;
}

void updateObject(Object* object) {
    AABB* aabb = object->getAABB();
    int index;
    
    double oldMinX = aabb->minX->value;
    double newMinX = object->getMinX();
    aabb->minX->value = newMinX;
    index = aabb->minX->index;
    if (oldMinX > newMinX) {
        while ()
    } else {

    }
        
    aabb->maxX->value = object->getMaxX();
    // TODO: Move in array
    aabb->minY->value = object->getMinY();
    // TODO: Move in array
    aabb->maxY->value = object->getMaxY();
    // TODO: Move in array
    aabb->minZ->value = object->getMinZ();
    // TODO: Move in array
    aabb->maxZ->value = object->getMaxZ();
    // TODO: Move in array
}