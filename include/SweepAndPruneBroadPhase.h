#ifndef SWEEPANDPRUNEBP_H
#define SWEEPANDPRUNEBP_H

#include "BroadPhaseAlgorithms.h"
#include "Plane.h"
#include "AABB.h"
#include "Helpers.h"

using namespace std;

class SweepAndPruneBroadPhase : public BroadPhaseAlgorithm {
private:
    map<string, pair<Object*, Object*>> collisionPairs;
    int pointsPerAxis;
    AABBPoint* xPoints;
    AABBPoint* yPoints;
    AABBPoint* zPoints;
    void insertObject(Object*);
    void updateAABBPoint(AABBPoint*, AABBPoint*);
    void insertAABBPoint(AABBPoint, int, AABBPoint*);
    bool AABBOverlapTest(AABB*, AABB*, AABBPoint*);
    void addCollision(Object*, Object*);
    void removeCollision(Object*, Object*);
public:
    SweepAndPruneBroadPhase(Object**, int);
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
    void updateObject(Object*);
};

#endif
