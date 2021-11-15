#ifndef NOBP_H
#define NOBP_H

#include "BroadPhaseAlgorithm.h"

using namespace std;

class NoBPCD : public BroadPhaseAlgorithm {
private:
    map<string, pair<Object*, Object*>>* collisionMap;
public:
    NoBPCD(vector<Object*>);
    map<string, pair<Object*, Object*>>* getCollisions(vector<Object*>);
};

#endif
