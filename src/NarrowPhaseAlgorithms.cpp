#include "../include/NarrowPhaseAlgorithms.h"

using namespace std;

pair<Point, Point>* NarrowPhaseAlgorithms::ballBall(Point center1, double radius1, Point center2, double radius2) {
    Point normalVector = center1 - center2;
    if (normalVector.getMagnitude() < radius1 + radius2) {
        double radiusRatio = radius1 / (radius1 + radius2);
        Point collisionPoint = center2 + normalVector * radiusRatio;
        return new pair<Point, Point>(collisionPoint, normalVector.normalize());
    }
    return NULL;
}

pair<Point, Point>* NarrowPhaseAlgorithms::ballCylinder(Point ballCenter, double ballRadius, Point cylinderCenter, double cylinderRadius, double cylinderLength, Point cylinderAxisDirection) {
    // https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
    Point AB = cylinderAxisDirection;
    Point AP = ballCenter - cylinderCenter;
    Point displacementFromA = AB * (AP.dotProduct(AB) / AB.dotProduct(AB));
    // Projection of the ball's center is inside the capsule's cylinder
    if (displacementFromA.getMagnitude() < cylinderLength / 2) {
        Point projection = cylinderCenter + displacementFromA;
        if ((ballCenter - projection).getMagnitude() < ballRadius + cylinderRadius) {
            Point collisionDirection = ballCenter - projection;
            Point collisionPoint = projection + (collisionDirection.normalize() * cylinderRadius);
            Point collisionNormal = projection - collisionPoint;
            return new pair<Point, Point>(collisionPoint, collisionNormal.normalize());
        }
    }
    return NULL;
}

pair<Point, Point>* NarrowPhaseAlgorithms::ballBall(Ball* ball1, Ball* ball2) {
    return ballBall(ball1->getPos(), ball1->getRadius(), ball2->getPos(), ball2->getRadius());
}

pair<Point, Point>* NarrowPhaseAlgorithms::ballCapsule(Ball* ball, Capsule* capsule) {
    pair<Point, Point>* cylinderCollision = ballCylinder(ball->getPos(), ball->getRadius(), capsule->getPos(), capsule->getRadius(), capsule->getLength(), capsule->getAxisDirection());
    if (cylinderCollision) return cylinderCollision;
    pair<Point, Point>* end1Collision = ballBall(ball->getPos(), ball->getRadius(), capsule->getCylinderEnd1(), capsule->getRadius());
    if (end1Collision) return end1Collision;
    return ballBall(ball->getPos(), ball->getRadius(), capsule->getCylinderEnd2(), capsule->getRadius());
}

pair<Point, Point>* NarrowPhaseAlgorithms::capsuleCapsule(Capsule* capsule1, Capsule* capsule2) {
    return NULL;
}

map<string, tuple<Object*, Object*, Point, Point>> NarrowPhaseAlgorithms::getCollisions(map<string, pair<Object*, Object*>> possibleCollisions) {
    map<string, tuple<Object*, Object*, Point, Point>> collisionMap;
    for (auto it = possibleCollisions.begin(); it != possibleCollisions.end(); it++) {
        Object* object1 = it->second.first;
        Object* object2 = it->second.second;

        pair<Point, Point>* collision = NULL;

        Ball* ball1 = dynamic_cast<Ball*>(object1);
        Ball* ball2 = dynamic_cast<Ball*>(object2);
        Capsule* capsule1 = dynamic_cast<Capsule*>(object1);
        Capsule* capsule2 = dynamic_cast<Capsule*>(object2);

        if (ball1 && ball2) {
            collision = ballBall(ball1, ball2);
        }
        else if (capsule1 && capsule2) {
            collision = capsuleCapsule(capsule1, capsule2);
        }
        else if (ball1 && capsule2) {
            collision = ballCapsule(ball1, capsule2);
        }
        else if (ball2 && capsule1) {
            collision = ballCapsule(ball2, capsule1);
            // Change normal direction so it points towards the second object
            if (collision) collision = new pair<Point, Point>(collision->first, collision->second * -1);
        }

        if (collision) {
            tuple<Object*, Object*, Point, Point> objectTuple;
            get<0>(objectTuple) = object1;
            get<1>(objectTuple) = object2;
            get<2>(objectTuple) = collision->first;
            get<3>(objectTuple) = collision->second;
            string objectPairId = getObjectPairId(make_pair(object1, object2));

            if (collisionMap.find(objectPairId) == collisionMap.end()) {
                collisionMap.insert(pair<string, tuple<Object*, Object*, Point, Point>>(objectPairId, objectTuple));
            }
        }
    }

    return collisionMap;
}
