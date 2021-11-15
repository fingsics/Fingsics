#ifndef BRUTEFORCEBP_H
#define BRUTEFORCEBP_H

#include "BroadPhaseAlgorithm.h"

using namespace std;

class AABBBruteForce : public BroadPhaseAlgorithm {
private:
    map<string, pair<Object*, Object*>>* collisionMap;
    bool AABBOverlapTest(Object*, Object*);
public:
    AABBBruteForce(vector<Object*>);
    map<string, pair<Object*, Object*>>* getCollisions(vector<Object*>);
};

#endif
