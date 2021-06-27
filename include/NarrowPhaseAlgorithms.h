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
    static map<string, tuple<Object*, Object*, Point, Point>> getCollisions(map<string, pair<Object*, Object*>>);
private:
    static pair<Point, Point>* ballBall(Ball*, Ball*);
    static pair<Point, Point>* ballCapsule(Ball*, Capsule*);
    static pair<Point, Point>* capsuleCapsule(Capsule*, Capsule*);
};

#endif
