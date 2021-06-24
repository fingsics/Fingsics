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

        Ball* ball1 = (Ball*)object1;
        Ball* ball2 = (Ball*)object2;
        Capsule* capsule1 = (Capsule*)object1;
        Capsule* capsule2 = (Capsule*)object2;

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
            pair<Object*, Object*> objectPair = make_pair(ball1, ball2);
            string objectPairId = getObjectPairId(objectPair);
            if (collisionMap.find(objectPairId) == collisionMap.end()) {
                collisionMap.insert(pair<string, pair<Object*, Object*>>(objectPairId, objectPair));
            }
        }
    }

    return collisionMap;
}