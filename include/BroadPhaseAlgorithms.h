#ifndef BPALG_H
#define BPALG_H
#include "Object.h"
#include "Ball.h"
#include "Helpers.h"
#include "AABB.h"
#include "Plane.h"
#include <map>
#include <string>
#include <unordered_set>

using namespace std;

class BroadPhaseAlgorithm {
public:
    virtual map<string, pair<Object*, Object*>> getCollisions(Object**, int) = 0;
};

class NoBroadPhase : public BroadPhaseAlgorithm {
public:
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};

class BruteForceBroadPhase : public BroadPhaseAlgorithm {
private:
    bool AABBOverlapTest(Object*, Object*);
public:
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};

class SweepAndPruneBroadPhase : public BroadPhaseAlgorithm {
private:
    map<string, pair<Object*, Object*>> collisionPairs;
    int pointsPerAxis;
    AABBPoint* xPoints;
    AABBPoint* yPoints;
    AABBPoint* zPoints;
    void insertAABBPoints(Object*, int);
public:
    SweepAndPruneBroadPhase(Object**, int);
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
    void updateObject(AABB*);
};

#endif
