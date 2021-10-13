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
    SAPAndOBBs(vector<Object*>);
    map<string, pair<Object*, Object*>> getCollisions(vector<Object*>);
};

#endif
