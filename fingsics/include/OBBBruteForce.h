#ifndef OBBBPCD_H
#define OBBBPCD_H

#include "BroadPhaseAlgorithm.h"

using namespace std;

class OBBBruteForce : public BroadPhaseAlgorithm {
private:
    map<string, pair<Object*, Object*>>* collisionMap;
    bool OBBOverlapTest(OBB, OBB);
public:
    OBBBruteForce(vector<Object*>);
    map<string, pair<Object*, Object*>>* getCollisions(vector<Object*>);
    map<string, pair<Object*, Object*>>* getCollisions(map<string, pair<Object*, Object*>>*);
};

#endif
