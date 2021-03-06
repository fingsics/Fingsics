#ifndef SWEEPANDPRUNEBP_H
#define SWEEPANDPRUNEBP_H

#include "BroadPhaseAlgorithm.h"
#include <unordered_set>

using namespace std;

class SweepAndPrune : public BroadPhaseAlgorithm {
private:
    map<string, pair<Object*, Object*>>* collisionMap;
    int pointsPerAxis;
    AABBPoint* xPoints;
    AABBPoint* yPoints;
    AABBPoint* zPoints;
    void insertObject(Object*, int);
    void fixAABBReference(AABBPoint*, AABBPoint*);
    void checkAABBPointForCollisions(AABBPoint*, AABBPoint*, unordered_set<Object*>&);
    void updateAABBPoint(AABBPoint*, AABBPoint*);
    void insertAABBPoint(AABBPoint, int, AABBPoint*);
    bool AABBOverlapTest(AABB*, AABB*, AABBPoint*);
    void addCollision(Object*, Object*);
    void removeCollision(Object*, Object*);
public:
    SweepAndPrune(vector<Object*>);
    map<string, pair<Object*, Object*>>* getCollisions(vector<Object*>);
    void updateObject(Object*);
};

#endif
