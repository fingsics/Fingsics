#include "../include/SweepAndPrune.h"

SweepAndPrune::SweepAndPrune(vector<Object*> objects) {
    this->collisionPairs = new map<string, pair<Object*, Object*>>();
    int numObjects = objects.size();
    this->pointsPerAxis = numObjects * 2;
    this->xPoints = new AABBPoint[2 * numObjects];
    this->yPoints = new AABBPoint[2 * numObjects];
    this->zPoints = new AABBPoint[2 * numObjects];
    int inserted = 0;

    // Create AABBs, insert values in AABBPoint arrays
    for (int i = 0; i < numObjects; i++) {
        insertObject(objects[i], i);
    }

    // Sort
    stable_sort(this->xPoints, this->xPoints + this->pointsPerAxis, [](const AABBPoint& a, const AABBPoint& b) { return a.value < b.value; });
    stable_sort(this->yPoints, this->yPoints + this->pointsPerAxis, [](const AABBPoint& a, const AABBPoint& b) { return a.value < b.value; });
    stable_sort(this->zPoints, this->zPoints + this->pointsPerAxis, [](const AABBPoint& a, const AABBPoint& b) { return a.value < b.value; });

    // Fix references to AABBPoints in AABBs
    for (int i = 0; i < this->pointsPerAxis; i++) {
        fixAABBReference(&xPoints[i], xPoints);
        fixAABBReference(&yPoints[i], yPoints);
        fixAABBReference(&zPoints[i], zPoints);
    };

    // Initialize collision collection
    unordered_set<Object*> openXObjects;
    unordered_set<Object*> openYObjects;
    unordered_set<Object*> openZObjects;
    for (int i = 0; i < this->pointsPerAxis; i++) {
        checkAABBPointForCollisions(&xPoints[i], xPoints, openXObjects);
        checkAABBPointForCollisions(&yPoints[i], yPoints, openYObjects);
        checkAABBPointForCollisions(&zPoints[i], zPoints, openZObjects);
    }
}

void SweepAndPrune::insertObject(Object* object, int index) {
    object->setAABB(new AABB(object));
    AABB* aabb = object->getAABB();

    // MinX
    xPoints[2 * index].isMin = true;
    xPoints[2 * index].aabb = aabb;
    xPoints[2 * index].value = object->getMinX();

    // MaxX
    xPoints[2 * index + 1].isMin = false;
    xPoints[2 * index + 1].aabb = aabb;
    xPoints[2 * index + 1].value = object->getMaxX();

    // MinY
    yPoints[2 * index].isMin = true;
    yPoints[2 * index].aabb = aabb;
    yPoints[2 * index].value = object->getMinY();

    // MaxY
    yPoints[2 * index + 1].isMin = false;
    yPoints[2 * index + 1].aabb = aabb;
    yPoints[2 * index + 1].value = object->getMaxY();

    // MinZ
    zPoints[2 * index].isMin = true;
    zPoints[2 * index].aabb = aabb;
    zPoints[2 * index].value = object->getMinZ();

    // MaxZ
    zPoints[2 * index + 1].isMin = false;
    zPoints[2 * index + 1].aabb = aabb;
    zPoints[2 * index + 1].value = object->getMaxZ();
}

void SweepAndPrune::fixAABBReference(AABBPoint* pointPointer, AABBPoint* pointArray) {
    if (pointPointer->isMin) {
        if (pointArray == xPoints) pointPointer->aabb->minX = pointPointer;
        if (pointArray == yPoints) pointPointer->aabb->minY = pointPointer;
        if (pointArray == zPoints) pointPointer->aabb->minZ = pointPointer;
    } else {
        if (pointArray == xPoints) pointPointer->aabb->maxX = pointPointer;
        if (pointArray == yPoints) pointPointer->aabb->maxY = pointPointer;
        if (pointArray == zPoints) pointPointer->aabb->maxZ = pointPointer;
    }
}

void SweepAndPrune::checkAABBPointForCollisions(AABBPoint* pointPointer, AABBPoint* pointArray, unordered_set<Object*>& openObjectsCollection) {
    if (openObjectsCollection.find(pointPointer->aabb->object) != openObjectsCollection.end()) {
        // Already open
        openObjectsCollection.erase(pointPointer->aabb->object);
    } else {
        // Not open
        for (unordered_set<Object*>::iterator it = openObjectsCollection.begin(); it != openObjectsCollection.end(); ++it) {
            if (AABBOverlapTest(pointPointer->aabb, (*it)->getAABB(), pointArray)) addCollision(pointPointer->aabb->object, *it);
        }
        openObjectsCollection.insert(pointPointer->aabb->object);
    }
}

void SweepAndPrune::updateObject(Object* object) {
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

void SweepAndPrune::updateAABBPoint(AABBPoint* pointPointer, AABBPoint* pointArray) {
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

void SweepAndPrune::insertAABBPoint(AABBPoint oldAABBPoint, int newIndex, AABBPoint* pointArray) {
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

bool SweepAndPrune::AABBOverlapTest(AABB* aabb1, AABB* aabb2, AABBPoint* pointArray) {
    if (pointArray != xPoints && (aabb1->minX > aabb2->maxX || aabb2->minX > aabb1->maxX)) return false;
    if (pointArray != yPoints && (aabb1->minY > aabb2->maxY || aabb2->minY > aabb1->maxY)) return false;
    if (pointArray != zPoints && (aabb1->minZ > aabb2->maxZ || aabb2->minZ > aabb1->maxZ)) return false;
    return true;
}

// SAP "Pair manager"

void SweepAndPrune::addCollision(Object* object1, Object* object2) {
    if (object1 == object2 || (object1->getIsStatic() && object2->getIsStatic())) return;
    pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(object1, object2);
    if (collisionPairs->find(objectPair.first) == collisionPairs->end()) {
        collisionPairs->insert(objectPair);
    }
}

void SweepAndPrune::removeCollision(Object* object1, Object* object2) {
    collisionPairs->erase(getObjectPairWithId(object1, object2).first);
}

map<string, pair<Object*, Object*>>* SweepAndPrune::getCollisions(vector<Object*> objects) {
    for (int i = 0; i < objects.size(); i++) {
        updateObject(objects[i]);
    }
    return collisionPairs;
}