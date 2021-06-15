#ifndef BPALG_H
#define BPALG_H
#include "Object.h"
#include <map>
#include <string>

using namespace std;

class BroadPhaseAlgorithm {
    protected:
        string getObjectPairId(pair<Object*, Object*> objectPair);
    public:
        // Virtual methods
        virtual map<string, pair<Object*, Object*>> getCollisions(Object** objects) = 0;
};

#endif
