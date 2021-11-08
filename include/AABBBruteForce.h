#ifndef BRUTEFORCEBP_H
#define BRUTEFORCEBP_H

#include "BroadPhaseAlgorithm.h"

using namespace std;

class AABBBruteForce : public BroadPhaseAlgorithm {
private:
    bool AABBOverlapTest(Object*, Object*);
public:
    map<string, pair<Object*, Object*>>* getCollisions(vector<Object*>);
};

#endif
