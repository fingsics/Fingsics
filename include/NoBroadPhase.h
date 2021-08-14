#ifndef NOBP_H
#define NOBP_H

#include "BroadPhaseAlgorithms.h"
#include "Helpers.h"

using namespace std;

class NoBroadPhase : public BroadPhaseAlgorithm {
public:
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};



#endif
