#ifndef SAPANDOBBS_H
#define SAPANDOBBS_H

#include "BroadPhaseAlgorithm.h"
#include "SweepAndPrune.h"
#include "OBBBruteForce.h"

using namespace std;

class SAPAndOBBs : public BroadPhaseAlgorithm {
private:
    SweepAndPrune* sap;
    OBBBruteForce* obbBruteForce;
public:
    SAPAndOBBs(Object**, int);
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};

#endif
