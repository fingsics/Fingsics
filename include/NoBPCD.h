#ifndef NOBP_H
#define NOBP_H

#include "BroadPhaseAlgorithm.h"

using namespace std;

class NoBPCD : public BroadPhaseAlgorithm {
public:
    map<string, pair<Object*, Object*>> getCollisions(vector<Object*>);
};

#endif
