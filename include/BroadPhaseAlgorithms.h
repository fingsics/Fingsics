#ifndef BPALG_H
#define BPALG_H
#include "Object.h"
#include "Ball.h"
#include "Helpers.h"
#include <map>
#include <string>

using namespace std;

class BroadPhaseAlgorithm {
public:
    virtual map<string, pair<Object*, Object*>> getCollisions(Object**, int) = 0;
};

class NoBroadPhase : public BroadPhaseAlgorithm {
public:
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};

class BruteForceBroadPhase : public BroadPhaseAlgorithm {
public:
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};

class SweepAndPruneBroadPhase : public BroadPhaseAlgorithm {
public:
    map<string, pair<Object*, Object*>> getCollisions(Object**, int);
};

#endif
