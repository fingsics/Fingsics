#ifndef BRUTEFORCEALG_H
#define BRUTEFORCEALG_H
#include "Object.h"
#include "Ball.h"
#include "BroadPhaseAlgorithm.h"
#include <map>
#include <string>

using namespace std;

class BruteForceBPA : public BroadPhaseAlgorithm {
    public:
        map<string, pair<Object*, Object*>> getCollisions(Object** objects);
};

#endif
