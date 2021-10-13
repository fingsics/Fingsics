#include "../include/SAPAndOBBs.h"

SAPAndOBBs::SAPAndOBBs(vector<Object*> objects) {
    sap = new SweepAndPrune(objects);
    obbBruteForce = new OBBBruteForce();
}

map<string, pair<Object*, Object*>> SAPAndOBBs::getCollisions(vector<Object*> objects) {
    map<string, pair<Object*, Object*>> SAPcollisions = sap->getCollisions(objects);
    map<string, pair<Object*, Object*>> SAPAndOBBColisions = obbBruteForce->getCollisions(SAPcollisions);
    return SAPAndOBBColisions;
}