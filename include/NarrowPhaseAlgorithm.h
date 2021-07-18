#ifndef NARROWPHASEALG_H
#define NARROWPHASEALG_H
#include "Object.h"
#include "Ball.h"
#include "Capsule.h"
#include "Plane.h"
#include "Helpers.h"
#include "Collision.h"
#include <map>
#include <string>

using namespace std;

class NarrowPhaseAlgorithm {
// All normals point towards the second object
public:
    map<string, Collision> getCollisions(map<string, pair<Object*, Object*>>);
private:
    map<string, Collision> lastFrameCollisions;
    Collision* ballPlane(Point, double, Point, Point);
    Collision* ballBall(Point, double, Point, double);
    Collision* ballCylinder(Point, double, Point, double, double, Point);
    Collision* ballCapsule(Point, double, Point, double, double, Point, Point, Point);
    Collision* ballBall(Ball*, Ball*);
    Collision* ballPlane(Ball*, Plane*);
    Collision* ballCapsule(Ball*, Capsule*);
    Collision* capsulePlane(Capsule*, Plane*);
    Collision* capsuleCapsule(Capsule*, Capsule*);
};

#endif
