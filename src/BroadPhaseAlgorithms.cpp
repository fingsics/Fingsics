#include "../include/BroadPhaseAlgorithms.h"

using namespace std;

// Adds every posible object pair to the map
map<string, pair<Object*, Object*>> NoBroadPhase::getCollisions(Object** objects, int numObjects) {
    map<string, pair<Object*, Object*>> collisionMap;
    for (int i = 0; i < numObjects; i++) {
        for (int j = i + 1; j < numObjects; j++) {
            if (objects[i]->getIsStatic() && objects[j]->getIsStatic()) continue;
            pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(objects[i], objects[j]);
            if (collisionMap.find(objectPair.first) == collisionMap.end()) {
                collisionMap.insert(objectPair);
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
            pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(objects[i], objects[j]);
            if (collisionMap.find(objectPair.first) == collisionMap.end() && (dynamic_cast<Plane*>(objects[i]) || dynamic_cast<Plane*>(objects[j]) || AABBOverlapTest(objects[i], objects[j]))) {
                collisionMap.insert(objectPair);
            }
        }
    }
    return collisionMap;
}

// SAP

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
        if (!dynamic_cast<Plane*>(objects[i])) insertObject (objects[i]);
    }
}

void SweepAndPruneBroadPhase::insertObject(Object* object) {
    AABB* newAABB = new AABB(object);

    // MinX
    xPoints[this->pointsPerAxis].isMin = true;
    xPoints[this->pointsPerAxis].aabb = newAABB;
    newAABB->minX = &xPoints[this->pointsPerAxis];

    // MaxX
    xPoints[this->pointsPerAxis + 1].isMin = false;
    xPoints[this->pointsPerAxis + 1].aabb = newAABB;
    newAABB->maxX = &xPoints[this->pointsPerAxis + 1];

    // MinY
    yPoints[this->pointsPerAxis].isMin = true;
    yPoints[this->pointsPerAxis].aabb = newAABB;
    newAABB->minY = &yPoints[this->pointsPerAxis];

    // MaxY
    yPoints[this->pointsPerAxis + 1].isMin = false;
    yPoints[this->pointsPerAxis + 1].aabb = newAABB;
    newAABB->maxY = &yPoints[this->pointsPerAxis + 1];

    // MinZ
    zPoints[this->pointsPerAxis].isMin = true;
    zPoints[this->pointsPerAxis].aabb = newAABB;
    newAABB->minZ = &zPoints[this->pointsPerAxis];

    // MaxZ
    zPoints[this->pointsPerAxis + 1].isMin = false;
    zPoints[this->pointsPerAxis + 1].aabb = newAABB;
    newAABB->maxZ = &zPoints[this->pointsPerAxis + 1];

    object->setAABB(newAABB);

    this->pointsPerAxis += 2;

    updateObject(object);
}

void SweepAndPruneBroadPhase::updateObject(Object* object) {
    // Ignore Planes in updates, since they're already in collisionPairs
    if (dynamic_cast<Plane*>(object)) return;

    AABB* aabb = object->getAABB();

    // MinX
    aabb->minX->value = object->getMinX();
    updateAABBPoint(aabb->minX, xPoints);

    // MaxX
    aabb->maxX->value = object->getMaxX();
    updateAABBPoint(aabb->maxX, xPoints);

    // MinY
    aabb->minY->value = object->getMinY();
    updateAABBPoint(aabb->minY, yPoints);

    // MaxY
    aabb->maxY->value = object->getMaxY();
    updateAABBPoint(aabb->maxY, yPoints);

    // MinZ
    aabb->minZ->value = object->getMinZ();
    updateAABBPoint(aabb->minZ, zPoints);

    // MaxZ
    aabb->maxZ->value = object->getMaxZ();
    updateAABBPoint(aabb->maxZ, zPoints);
}

void SweepAndPruneBroadPhase::updateAABBPoint(AABBPoint* pointPointer, AABBPoint* pointArray) {
    int index = pointPointer - pointArray;
    AABBPoint point = AABBPoint(pointPointer);

    // Shift other points to the right (updated point is smaller and moving to the left)
    while (index > 0 && point.value < pointArray[index - 1].value) {
        if (!point.isMin && pointArray[index - 1].isMin) {
            removeCollision(point.aabb->object, pointArray[index - 1].aabb->object);
        } else if (point.isMin && !pointArray[index - 1].isMin && AABBOverlapTest(point.aabb, pointArray[index - 1].aabb, pointArray)) {
            addCollision(point.aabb->object, pointArray[index - 1].aabb->object);
        }

        insertAABBPoint(pointArray[index - 1], index, pointArray);

        index--;
    }
    
    // Shift other points to the left (updated point is bigger and moving to the right)
    while (index < pointsPerAxis - 1 && point.value > pointArray[index + 1].value) {
        if (point.isMin && !pointArray[index + 1].isMin) {
            removeCollision(point.aabb->object, pointArray[index + 1].aabb->object);
        } else if (!point.isMin && pointArray[index + 1].isMin && AABBOverlapTest(point.aabb, pointArray[index + 1].aabb, pointArray)) {
            addCollision(point.aabb->object, pointArray[index + 1].aabb->object);
        }

        insertAABBPoint(pointArray[index + 1], index, pointArray);

        index++;
    }
    
    // Place point in final position
    if (index != pointPointer - pointArray) insertAABBPoint(point, index, pointArray);
}

void SweepAndPruneBroadPhase::insertAABBPoint(AABBPoint oldAABBPoint, int newIndex, AABBPoint* pointArray) {
    pointArray[newIndex].value = oldAABBPoint.value;
    pointArray[newIndex].isMin = oldAABBPoint.isMin;
    pointArray[newIndex].aabb = oldAABBPoint.aabb;

    // Update corresponding AABB
    if (pointArray[newIndex].isMin) {
        if (pointArray == xPoints) pointArray[newIndex].aabb->minX = pointArray + newIndex;
        else if (pointArray == yPoints) pointArray[newIndex].aabb->minY = pointArray + newIndex;
        else if (pointArray == zPoints) pointArray[newIndex].aabb->minZ = pointArray + newIndex;
    }
    else {
        if (pointArray == xPoints) pointArray[newIndex].aabb->maxX = pointArray + newIndex;
        else if (pointArray == yPoints) pointArray[newIndex].aabb->maxY = pointArray + newIndex;
        else if (pointArray == zPoints) pointArray[newIndex].aabb->maxZ = pointArray + newIndex;
    }
}

bool SweepAndPruneBroadPhase::AABBOverlapTest(AABB* aabb1, AABB* aabb2, AABBPoint* pointArray) {
    if (pointArray != xPoints && (aabb1->minX > aabb2->maxX || aabb2->minX > aabb1->maxX)) return false;
    if (pointArray != yPoints && (aabb1->minY > aabb2->maxY || aabb2->minY > aabb1->maxY)) return false;
    if (pointArray != zPoints && (aabb1->minZ > aabb2->maxZ || aabb2->minZ > aabb1->maxZ)) return false;
    return true;
}

// SAP "Pair manager"

void SweepAndPruneBroadPhase::addCollision(Object* object1, Object* object2) {
    if (object1 == object2 || (object1->getIsStatic() && object2->getIsStatic())) return;
    pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(object1, object2);
    if (collisionPairs.find(objectPair.first) == collisionPairs.end()) {
        collisionPairs.insert(objectPair);
    }
}

void SweepAndPruneBroadPhase::removeCollision(Object* object1, Object* object2) {
    collisionPairs.erase(getObjectPairWithId(object1, object2).first);
}

map<string, pair<Object*, Object*>> SweepAndPruneBroadPhase::getCollisions(Object** objects, int numObjects) {
    for (int i = 0; i < numObjects; i++) {
        updateObject(objects[i]);
    }
    return collisionPairs;
}