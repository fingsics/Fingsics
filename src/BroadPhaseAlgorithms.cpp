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

bool SweepAndPruneBroadPhase::AABBOverlapTest(AABB* aabb1, AABB* aabb2) {
    if (aabb1->minX.value > aabb2->maxX.value || aabb2->minX.value > aabb1->maxX.value) return false;
    if (aabb1->minY.value > aabb2->maxY.value || aabb2->minY.value > aabb1->maxY.value) return false;
    if (aabb1->minZ.value > aabb2->maxZ.value || aabb2->minZ.value > aabb1->maxZ.value) return false;
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
    AABB* newAABB = new AABB(object);
    newAABB->minX.index = insertedCount;
    newAABB->maxX.index = insertedCount + 1;
    newAABB->minY.index = insertedCount;
    newAABB->maxY.index = insertedCount + 1;
    newAABB->minZ.index = insertedCount;
    newAABB->maxZ.index = insertedCount + 1;
    xPoints[insertedCount] = newAABB->minX;
    xPoints[insertedCount + 1] = newAABB->maxX;
    yPoints[insertedCount] = newAABB->minY;
    yPoints[insertedCount + 1] = newAABB->maxY;
    zPoints[insertedCount] = newAABB->minZ;
    zPoints[insertedCount + 1] = newAABB->maxZ;
    object->setAABB(newAABB);
    updateObject(object);
}

void SweepAndPruneBroadPhase::addCollision(Object* object1, Object* object2) {
    pair<Object*, Object*> objectPair = make_pair(object1, object2);
    string objectPairId = getObjectPairId(objectPair);
    if (collisionPairs.find(objectPairId) == collisionPairs.end()) {
        collisionPairs.insert(pair<string, pair<Object*, Object*>>(objectPairId, objectPair));
    }
}

void SweepAndPruneBroadPhase::removeCollision(Object* object1, Object* object2) {
    collisionPairs.erase(getObjectPairId(make_pair(object1, object2)));
}

SweepAndPruneBroadPhase::SweepAndPruneBroadPhase(Object** objects, int numObjects) {
    // Count Planes and add all their collisions to the list (these won't be discarded by SAP)
    int nonPlaneObjects = numObjects;
    for (int i = 0; i < numObjects; i++) {
        if (dynamic_cast<Plane*>(objects[i])) {
            nonPlaneObjects--;
            // Plane-Plane collisions are ignored by NarrowPhase, they are added for consistency with NoBroadPhase and BruteForceBroadPhase
            for (int j = 0; j < numObjects; j++) if (i != j) addCollision(objects[i], objects[j]);
        }
    }

    this->xPoints = new AABBPoint[2 * nonPlaneObjects];
    this->yPoints = new AABBPoint[2 * nonPlaneObjects];
    this->zPoints = new AABBPoint[2 * nonPlaneObjects];
    
    // Add non-Planes to the SAP arrays
    this->pointsPerAxis = 0;
    for (int i = 0; i < numObjects; i++) {
        if (!dynamic_cast<Plane*>(objects[i])) {
            insertAABBPoints(objects[i], this->pointsPerAxis);
            this->pointsPerAxis += 2;
        }
    }
}

map<string, pair<Object*, Object*>> SweepAndPruneBroadPhase::getCollisions(Object** objects, int numObjects) {
    for (int i = 0; i < numObjects; i++) updateObject(objects[i]);
    return collisionPairs;
}

int SweepAndPruneBroadPhase::updateAABBPoint(Object* object, AABBPoint point, double oldValue, AABBPoint* pointArray) {
    int index = point.index;
    if (point.value < oldValue) {
        while (index > 0) {
            AABBPoint previous = pointArray[index - 1];
            if (previous.value <= point.value) break;

            if (point.isMin && !previous.isMin && AABBOverlapTest(object->getAABB(), previous.object->getAABB())) {
                addCollision(object, previous.object);
            }
            else if (!point.isMin && previous.isMin) {
                removeCollision(object, previous.object);
            }
            pointArray[index] = previous;
            index--;
        }
    } else {
        while (index < pointsPerAxis - 1) {
            AABBPoint next = pointArray[index + 1];
            if (next.value >= point.value) break;

            if (point.isMin && !next.isMin) {
                removeCollision(object, next.object);
            }
            else if (!point.isMin && next.isMin && AABBOverlapTest(object->getAABB(), next.object->getAABB())) {
                addCollision(object, next.object);
            }
            pointArray[index] = next;
            index++;
        }
    }
    point.index = index;
    pointArray[index] = point;
    return index;
}

void SweepAndPruneBroadPhase::updateObject(Object* object) {
    // Ignore Planes in updates, since they're already in collisionPairs
    if (dynamic_cast<Plane*>(object)) return;
    
    AABB* aabb = object->getAABB();

    // Update minX
    double oldValue = aabb->minX.value;
    aabb->minX.value = object->getMinX();
    aabb->minX.index = updateAABBPoint(object, aabb->minX, oldValue, xPoints);

    // Update maxX
    oldValue = aabb->maxX.value;
    aabb->maxX.value = object->getMaxX();
    aabb->maxX.index = updateAABBPoint(object, aabb->maxX, oldValue, xPoints);

    // Update minY
    oldValue = aabb->minY.value;
    aabb->minY.value = object->getMinY();
    aabb->minY.index = updateAABBPoint(object, aabb->minY, oldValue, yPoints);

    // Update maxY
    oldValue = aabb->maxY.value;
    aabb->maxY.value = object->getMaxY();
    aabb->maxY.index = updateAABBPoint(object, aabb->maxY, oldValue, yPoints);

    // Update minZ
    oldValue = aabb->minZ.value;
    aabb->minZ.value = object->getMinZ();
    aabb->minZ.index = updateAABBPoint(object, aabb->minZ, oldValue, zPoints);

    // Update maxZ
    oldValue = aabb->maxZ.value;
    aabb->maxZ.value = object->getMaxZ();
    aabb->maxZ.index = updateAABBPoint(object, aabb->maxZ, oldValue, zPoints);
}