#include "../include/MultithreadSweepAndPruneBroadPhase.h"

MultithreadSweepAndPruneBroadPhase::MultithreadSweepAndPruneBroadPhase(Object** objects, int numObjects) {
    // Count Planes and add all their collisions to the list (these won't be discarded by SAP)
    int nonPlaneObjects = numObjects;
    for (int i = 0; i < numObjects; i++) {
        if (dynamic_cast<Plane*>(objects[i])) {
            nonPlaneObjects--;
            // Plane-Plane collisions are ignored by NarrowPhase, they are added for consistency with NoBroadPhase and BruteForceBroadPhase
            for (int j = 0; j < numObjects; j++) if (i != j) addCollision(objects[i], objects[j], false);
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

void MultithreadSweepAndPruneBroadPhase::insertObject(Object* object) {
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

    // MinX
    newAABB->minX->value = object->getMinX();
    updateAABBPoint(newAABB->minX, xPoints);

    // MaxX
    newAABB->maxX->value = object->getMaxX();
    updateAABBPoint(newAABB->maxX, xPoints);

    // MinY
    newAABB->minY->value = object->getMinY();
    updateAABBPoint(newAABB->minY, yPoints);

    // MaxY
    newAABB->maxY->value = object->getMaxY();
    updateAABBPoint(newAABB->maxY, yPoints);

    // MinZ
    newAABB->minZ->value = object->getMinZ();
    updateAABBPoint(newAABB->minZ, zPoints);

    // MaxZ
    newAABB->maxZ->value = object->getMaxZ();
    updateAABBPoint(newAABB->maxZ, zPoints);
}

void MultithreadSweepAndPruneBroadPhase::updateXAxis(Object** objects, int numObjects) {
    AABB* aabb = NULL;
    for (int i = 0; i < numObjects; i++) {
        if (dynamic_cast<Plane*>(objects[i])) return;
        aabb = objects[i]->getAABB();

        // MinX
        aabb->minX->value = objects[i]->getMinX();
        updateAABBPoint(aabb->minX, xPoints);

        // MaxX
        aabb->maxX->value = objects[i]->getMaxX();
        updateAABBPoint(aabb->maxX, xPoints);
    }
}

void MultithreadSweepAndPruneBroadPhase::updateYAxis(Object** objects, int numObjects) {
    AABB* aabb = NULL;
    for (int i = 0; i < numObjects; i++) {
        if (dynamic_cast<Plane*>(objects[i])) return;
        aabb = objects[i]->getAABB();

        // MinY
        aabb->minY->value = objects[i]->getMinY();
        updateAABBPoint(aabb->minY, yPoints);

        // MaxY
        aabb->maxY->value = objects[i]->getMaxY();
        updateAABBPoint(aabb->maxY, yPoints);
    }
}

void MultithreadSweepAndPruneBroadPhase::updateZAxis(Object** objects, int numObjects) {
    AABB* aabb = NULL;
    for (int i = 0; i < numObjects; i++) {
        if (dynamic_cast<Plane*>(objects[i])) return;
        aabb = objects[i]->getAABB();

        // MinZ
        aabb->minZ->value = objects[i]->getMinZ();
        updateAABBPoint(aabb->minZ, zPoints);

        // MaxZ
        aabb->maxZ->value = objects[i]->getMaxZ();
        updateAABBPoint(aabb->maxZ, zPoints);
    }
}

void MultithreadSweepAndPruneBroadPhase::updateAABBPoint(AABBPoint* pointPointer, AABBPoint* pointArray) {
    int index = pointPointer - pointArray;
    AABBPoint point = AABBPoint(pointPointer);

    // Shift other points to the right (updated point is smaller and moving to the left)
    while (index > 0 && point.value < pointArray[index - 1].value) {
        if (!point.isMin && pointArray[index - 1].isMin) {
            removeCollision(point.aabb->object, pointArray[index - 1].aabb->object);
        } else if (point.isMin && !pointArray[index - 1].isMin && AABBOverlapTest(point.aabb, pointArray[index - 1].aabb, pointArray)) {
            addCollision(point.aabb->object, pointArray[index - 1].aabb->object, false);
        }

        insertAABBPoint(pointArray[index - 1], index, pointArray);

        index--;
    }
    
    // Shift other points to the left (updated point is bigger and moving to the right)
    while (index < pointsPerAxis - 1 && point.value > pointArray[index + 1].value) {
        if (point.isMin && !pointArray[index + 1].isMin) {
            removeCollision(point.aabb->object, pointArray[index + 1].aabb->object);
        } else if (!point.isMin && pointArray[index + 1].isMin && AABBOverlapTest(point.aabb, pointArray[index + 1].aabb, pointArray)) {
            addCollision(point.aabb->object, pointArray[index + 1].aabb->object, false);
        }

        insertAABBPoint(pointArray[index + 1], index, pointArray);

        index++;
    }
    
    // Place point in final position
    if (index != pointPointer - pointArray) insertAABBPoint(point, index, pointArray);
}


void MultithreadSweepAndPruneBroadPhase::updateAABBPointInAxis(AABBPoint* pointPointer, AABBPoint* pointArray) {
    int index = pointPointer - pointArray;
    AABBPoint point = AABBPoint(pointPointer);

    // Shift other points to the right (updated point is smaller and moving to the left)
    while (index > 0 && point.value < pointArray[index - 1].value) {
        if ((!point.isMin && pointArray[index - 1].isMin) || (point.isMin && !pointArray[index - 1].isMin)) {

            addCollision(point.aabb->object, pointArray[index - 1].aabb->object, true);
        }

        insertAABBPoint(pointArray[index - 1], index, pointArray);

        index--;
    }

    // Shift other points to the left (updated point is bigger and moving to the right)
    while (index < pointsPerAxis - 1 && point.value > pointArray[index + 1].value) {
        if ((point.isMin && !pointArray[index + 1].isMin) || (!point.isMin && pointArray[index + 1].isMin)) {
            addCollision(point.aabb->object, pointArray[index + 1].aabb->object, true);
        }

        insertAABBPoint(pointArray[index + 1], index, pointArray);

        index++;
    }

    // Place point in final position
    if (index != pointPointer - pointArray) insertAABBPoint(point, index, pointArray);
}

void MultithreadSweepAndPruneBroadPhase::insertAABBPoint(AABBPoint oldAABBPoint, int newIndex, AABBPoint* pointArray) {
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

bool MultithreadSweepAndPruneBroadPhase::AABBOverlapTest(AABB* aabb1, AABB* aabb2, AABBPoint* pointArray) {
    if (pointArray != xPoints && (aabb1->minX > aabb2->maxX || aabb2->minX > aabb1->maxX)) return false;
    if (pointArray != yPoints && (aabb1->minY > aabb2->maxY || aabb2->minY > aabb1->maxY)) return false;
    if (pointArray != zPoints && (aabb1->minZ > aabb2->maxZ || aabb2->minZ > aabb1->maxZ)) return false;
    return true;
}

// SAP "Pair manager"

void MultithreadSweepAndPruneBroadPhase::addCollision(Object* object1, Object* object2, bool frameOnly) {
    if (object1 == object2 || (object1->getIsStatic() && object2->getIsStatic())) return;
    pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(object1, object2);
    if (!frameOnly) {
        if (collisionPairs.find(objectPair.first) == collisionPairs.end()) collisionPairs.insert(objectPair);
    } else {
        mtx.lock();
        if (frameDetections.find(objectPair.first) == frameDetections.end()) frameDetections.insert(objectPair);
        mtx.unlock();
    }
}

void MultithreadSweepAndPruneBroadPhase::removeCollision(Object* object1, Object* object2) {
    collisionPairs.erase(getObjectPairWithId(object1, object2).first);
}

map<string, pair<Object*, Object*>> MultithreadSweepAndPruneBroadPhase::getCollisions(Object** objects, int numObjects) {
    thread xAxisWorker(&MultithreadSweepAndPruneBroadPhase::updateXAxis, this, objects, numObjects);
    thread yAxisWorker(&MultithreadSweepAndPruneBroadPhase::updateYAxis, this, objects, numObjects);
    thread zAxisWorker(&MultithreadSweepAndPruneBroadPhase::updateZAxis, this, objects, numObjects);

    xAxisWorker.join();
    yAxisWorker.join();
    zAxisWorker.join();

    

    for (auto change = frameDetections.begin(); change != frameDetections.end(); ++change) {
        if (AABBOverlapTest(change->second.first->getAABB(), change->second.second->getAABB(), NULL)) {
            addCollision(change->second.first, change->second.second, false);
        } else {
            removeCollision(change->second.first, change->second.second);
        }
    }
    frameDetections.clear();

    return collisionPairs;
}