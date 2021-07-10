#ifndef NARROWPHASEALG_H
#define NARROWPHASEALG_H
#include "Object.h"
#include "Ball.h"
#include "Capsule.h"
#include "Plane.h"
#include "Helpers.h"
#include <map>
#include <string>

using namespace std;

class NarrowPhaseAlgorithms {
// All normals point towards the second object
public:
    static map<string, tuple<Object*, Object*, Point, Point>> getCollisions(map<string, pair<Object*, Object*>>);
private:
    static pair<Point, Point>* ballPlane(Point, double, Point, Point);
    static pair<Point, Point>* ballBall(Point, double, Point, double);
    static pair<Point, Point>* ballCylinder(Point, double, Point, double, double, Point);
    static pair<Point, Point>* cylinderCylinder(Point, double, double, Point, Point, double, double, Point);
    static pair<Point, Point>* ballCapsule(Point ballPos, double ballRadius, Point capsulePos, double capsuleRadius, double capsuleLength, Point capsuleAxisDirection, Point capsulePositiveEnd, Point capsuleNegativeEnd);
    static pair<Point, Point>* ballBall(Ball*, Ball*);
    static pair<Point, Point>* ballPlane(Ball*, Plane*);
    static pair<Point, Point>* ballCapsule(Ball*, Capsule*);
    static pair<Point, Point>* capsulePlane(Capsule*, Plane*);
    static pair<Point, Point>* capsuleCapsule(Capsule*, Capsule*);
};

#endif
