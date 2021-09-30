#ifndef SWEEPANDPRUNEBP_H
#define SWEEPANDPRUNEBP_H

#include "BroadPhaseAlgorithm.h"

using namespace std;

class SweepAndPrune : public BroadPhaseAlgorithm {
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
    SweepAndPrune(Object**, int);
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
    void updateObject(Object*);
};

#endif
