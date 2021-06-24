#ifndef MPALG_H
#define MPALG_H
#include "Object.h"
#include "Ball.h"
#include "Helpers.h"
#include <map>
#include <string>

using namespace std;

class MidPhaseAlgorithm {
public:
    virtual map<string, pair<Object*, Object*>> getCollisions(map<string, pair<Object*, Object*>>) = 0;
};

class NoMidPhase : public MidPhaseAlgorithm {
public:
    map<string, pair<Object*, Object*>> getCollisions(map<string, pair<Object*, Object*>>);
};

class OBBMidPhase : public MidPhaseAlgorithm {
public:
    map<string, pair<Object*, Object*>> getCollisions(map<string, pair<Object*, Object*>>);
};

#endif
