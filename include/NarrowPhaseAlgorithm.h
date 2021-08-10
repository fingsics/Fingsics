#ifndef NARROWPHASEALG_H
#define NARROWPHASEALG_H
#include "Object.h"
#include "Ball.h"
#include "Capsule.h"
#include "Plane.h"
#include "Tile.h"
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
    Collision* ballLine(Point, float, Point, Point, float);
    Collision* ballPlane(Point, float, Point, Point);
    Collision* ballBall(Point, float, Point, float);
    Collision* ballCylinder(Point, float, Point, float, float, Point);
    Collision* ballCapsule(Point, float, Point, float, float, Point, Point, Point);
    Collision* ballBall(Ball*, Ball*);
    Collision* ballPlane(Ball*, Plane*);
    Collision* ballTile(Ball*, Tile*);
    Collision* ballCapsule(Ball*, Capsule*);
    Collision* capsulePlane(Capsule*, Plane*);
    Collision* capsuleCapsule(Capsule*, Capsule*);
    Collision* capsuleTile(Capsule*, Tile*);
};

#endif
