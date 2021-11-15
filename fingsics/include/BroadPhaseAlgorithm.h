#ifndef BPALG_H
#define BPALG_H

#include "Object.h"
#include "Helpers.h"
#include "AABB.h"
#include <map>
#include <string>

using namespace std;


class BroadPhaseAlgorithm {
public:
    virtual map<string, pair<Object*, Object*>>* getCollisions(vector<Object*>) = 0;
};

#endif
