#ifndef BRUTEFORCEBP_H
#define BRUTEFORCEBP_H

#include "BroadPhaseAlgorithms.h"
#include "AABB.h"
#include "Helpers.h"

using namespace std;

class BruteForceBroadPhase : public BroadPhaseAlgorithm {
private:
    bool AABBOverlapTest(Object*, Object*);
public:
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};

#endif
