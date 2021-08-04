#ifndef MULTITHREADEDSWEEPANDPRUNEBP_H
#define MULTITHREADEDSWEEPANDPRUNEBP_H

#include "BroadPhaseAlgorithms.h"
#include "Plane.h"
#include "AABB.h"
#include "Helpers.h"
#include <thread>
#include <mutex>

using namespace std;

class MultithreadSweepAndPruneBroadPhase : public BroadPhaseAlgorithm {
private:
    map<string, pair<Object*, Object*>> collisionPairs;
    mutex mtx;
    int pointsPerAxis;
    AABBPoint* xPoints;
    AABBPoint* yPoints;
    AABBPoint* zPoints;
    void insertObject(Object*);
    void updateXAxis(Object**, int);
    void updateYAxis(Object**, int);
    void updateZAxis(Object**, int);
    void updateAABBPoint(AABBPoint*, AABBPoint*);
    void updateAABBPointInAxis(AABBPoint*, AABBPoint*);
    void insertAABBPoint(AABBPoint, int, AABBPoint*);
    bool AABBOverlapTest(Object*, Object*, AABBPoint*);
    void addCollision(Object*, Object*);
    void removeCollision(Object*, Object*);
public:
    MultithreadSweepAndPruneBroadPhase(Object**, int);
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};


#include "BroadPhaseAlgorithms.h"

#endif
