#ifndef MPALG_H
#define MPALG_H
#include "Object.h"
#include "Ball.h"
#include "Point.h"
#include "Matrix.h"
#include "Helpers.h"
#include "OBB.h"
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
private:
    bool OBBOverlapTest(OBB, OBB);
public:
    map<string, pair<Object*, Object*>> getCollisions(map<string, pair<Object*, Object*>>);
};

#endif
