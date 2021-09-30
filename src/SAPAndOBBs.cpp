#include "../include/SAPAndOBBs.h"

SAPAndOBBs::SAPAndOBBs(Object** objects, int numObjects) {
    sap = new SweepAndPrune(objects, numObjects);
    obbBruteForce = new OBBBruteForce();
}

map<string, pair<Object*, Object*>> SAPAndOBBs::getCollisions(Object** objects, int numObjects) {
    map<string, pair<Object*, Object*>> SAPcollisions = sap->getCollisions(objects, numObjects);
    map<string, pair<Object*, Object*>> SAPAndOBBColisions = obbBruteForce->getCollisions(SAPcollisions);
    return SAPAndOBBColisions;
}