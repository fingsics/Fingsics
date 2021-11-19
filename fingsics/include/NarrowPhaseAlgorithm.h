#ifndef NARROWPHASEALG_H
#define NARROWPHASEALG_H

#include "Object.h"
#include "Ball.h"
#include "Capsule.h"
#include "Tile.h"
#include "Helpers.h"
#include "Collision.h"
#include "NPCDData.h"
#include <map>
#include <string>
#include <chrono>

using namespace std;

class NarrowPhaseAlgorithm {
// All normals point towards the second object
public:
    pair<map<string, Collision>, NPCDData> getCollisions(map<string, pair<Object*, Object*>>*);
private:
    map<string, Collision> lastFrameCollisions;
    Collision* ballLine(Vector, float, Vector, Vector, float);
    Collision* ballBall(Vector, float, Vector, float);
    Collision* ballCylinder(Vector, float, Vector, float, float, Vector);
    Collision* ballCapsule(Vector, float, Vector, float, float, Vector, Vector, Vector);
    Collision* ballTile(Vector, float, Vector, Vector, Vector, Vector, float, float, Vector, Vector, Vector, Vector);
    Collision* ballBall(Ball*, Ball*);
    Collision* ballTile(Ball*, Tile*);
    Collision* ballCapsule(Ball*, Capsule*);
    Collision* capsuleCapsule(Capsule*, Capsule*);
    Collision* capsuleTile(Capsule*, Tile*);
    Collision* parallelCapsules(Capsule*, Capsule*);
    tuple<Vector, Vector>* calculateCylinderLineCollision(Capsule*, Vector, Vector, float, tuple<float, float, float>);
    tuple<float, float, float> closestPointBetweenNonParallelLines(Vector, Vector, Vector, Vector, Vector);
    tuple<tuple<Vector, Vector>*, tuple<Vector, Vector>*> parallelCapsuleAndTileEdgeCollisions(Vector, Vector, Vector, Vector, float);
};

#endif
