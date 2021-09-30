#ifndef OBBBPCD_H
#define OBBBPCD_H

#include "BroadPhaseAlgorithm.h"

using namespace std;

class OBBBruteForce : public BroadPhaseAlgorithm {
private:
    bool OBBOverlapTest(OBB, OBB);
public:
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
    map<string, pair<Object*, Object*>> getCollisions(map<string, pair<Object*, Object*>>);
};

#endif
