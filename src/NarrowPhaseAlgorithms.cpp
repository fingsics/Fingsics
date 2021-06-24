#include "../include/NarrowPhaseAlgorithms.h"

using namespace std;

bool NarrowPhaseAlgorithms::ballBall(Ball* ball1, Ball* ball2) {
    Point object1Pos = ball1->getPos();
    Point object2Pos = ball2->getPos();

    Point normalVector = object1Pos - object2Pos;
    double distance = normalVector.magnitude();

    return distance < ball1->getRad() + ball2->getRad();
}

bool NarrowPhaseAlgorithms::ballCapsule(Ball* ball, Capsule* capsule) {
    // https://gamedev.stackexchange.com/questions/72528/how-can-i-project-a-3d-point-onto-a-3d-line
    Point AB = capsule->getAxisDirection();
    Point AP = ball->getPos() - capsule->getPos();
    Point displacementFromA = AB * (AP.dotProduct(AB) / AB.dotProduct(AB));
    if (displacementFromA.magnitude() < capsule->getLength() / 2) {
        // Projection of the ball's center is inside the capsule's cylinder
        Point projection = capsule->getPos() + displacementFromA;
        Point dist = ball->getPos() - projection;
        double distttt = dist.magnitude();
        return distttt < ball->getRad() + capsule->getRadius();
    } else {
        // Projection of the ball's center is outside of the capsule's cylinder
        return (ball->getPos() - capsule->getCylinderEnd1()).magnitude() < ball->getRad() + capsule->getRadius()
            || (ball->getPos() - capsule->getCylinderEnd2()).magnitude() < ball->getRad() + capsule->getRadius();
    }
    return false;
}

bool NarrowPhaseAlgorithms::capsuleCapsule(Capsule* capsule1, Capsule* capsule2) {
    return false;
}

map<string, pair<Object*, Object*>> NarrowPhaseAlgorithms::getCollisions(map<string, pair<Object*, Object*>> possibleCollisions) {
    map<string, pair<Object*, Object*>> collisionMap;
    for (auto it = possibleCollisions.begin(); it != possibleCollisions.end(); it++) {
        Object* object1 = it->second.first;
        Object* object2 = it->second.second;

        bool collision = false;

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
        }

        if (collision) {
            pair<Object*, Object*> objectPair = make_pair(object1, object2);
            string objectPairId = getObjectPairId(objectPair);
            if (collisionMap.find(objectPairId) == collisionMap.end()) {
                collisionMap.insert(pair<string, pair<Object*, Object*>>(objectPairId, objectPair));
            }
        }
    }

    return collisionMap;
}