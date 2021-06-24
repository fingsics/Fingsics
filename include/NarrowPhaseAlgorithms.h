#ifndef NARROWPHASEALG_H
#define NARROWPHASEALG_H
#include "Object.h"
#include "Ball.h"
#include "Capsule.h"
#include "Helpers.h"
#include <map>
#include <string>

using namespace std;

class NarrowPhaseAlgorithms {
public:
    static map<string, pair<Object*, Object*>> getCollisions(map<string, pair<Object*, Object*>>, int);
private:
    static bool ballBall(Ball*, Ball*);
    static bool ballCapsule(Ball*, Capsule*);
    static bool capsuleCapsule(Capsule*, Capsule*);
};

#endif
