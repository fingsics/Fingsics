#ifndef BPALG_H
#define BPALG_H

#include "Object.h"
#include <map>
#include <string>
#include "Plane.h"
#include "Helpers.h"

using namespace std;


class BroadPhaseAlgorithm {
public:
    virtual map<string, pair<Object*, Object*>> getCollisions(Object**, int) = 0;
};

#endif
