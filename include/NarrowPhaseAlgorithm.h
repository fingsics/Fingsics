#ifndef NARROWPHASEALG_H
#define NARROWPHASEALG_H
#include "Object.h"
#include "Ball.h"
#include "Capsule.h"
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
    Collision* ballBall(Point, float, Point, float);
    Collision* ballCylinder(Point, float, Point, float, float, Point);
    Collision* ballCapsule(Point, float, Point, float, float, Point, Point, Point);
    Collision* ballTile(Point, float, Point, Point, Point, Point, float, float, Point, Point, Point, Point);
    Collision* ballBall(Ball*, Ball*);
    Collision* ballTile(Ball*, Tile*);
    Collision* ballCapsule(Ball*, Capsule*);
    Collision* capsuleCapsule(Capsule*, Capsule*);
    Collision* capsuleTile(Capsule*, Tile*);
    Collision* parallelCapsules(Capsule*, Capsule*);
    tuple<Point, Point>* calculateCylinderLineCollision(Capsule*, Point, Point, float, tuple<float, float, float>);
    tuple<float, float, float> closestPointBetweenNonParallelLines(Point, Point, Point, Point, Point);
    tuple<tuple<Point, Point>*, tuple<Point, Point>*> parallelCapsuleAndTileEdgeCollisions(Point, Point, Point, Point, float);
};

#endif
