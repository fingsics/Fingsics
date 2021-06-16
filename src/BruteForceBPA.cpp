#include "../include/BruteForceBPA.h"

using namespace std;

map<string, pair<Object*, Object*>> BruteForceBPA::getCollisions(Object** objects, int numObjects) {
    map<string, pair<Object*, Object*>> collisionMap;
    for (int i = 0; i < numObjects; i++)
        for (int j = i + 1; j < numObjects; j++) {
            Object* object1 = objects[i];
            Object* object2 = objects[j];
            Point object1Pos = object1->getPos();
            Point object2Pos = object2->getPos();

            Point normalVector = object1Pos - object2Pos;
            double distance = normalVector.magnitude();

            Ball* ball1 = (Ball*)object1;
            Ball* ball2 = (Ball*)object2;

            if (ball1 && ball2) {
                // Ball Collision detection
                if (distance < ball1->getRad() + ball2->getRad()) {
                    pair<Object*, Object*> objectPair = make_pair(ball1, ball2);
                    string objectPairId = getObjectPairId(objectPair);
                    if (collisionMap.find(objectPairId) == collisionMap.end()) {
                        collisionMap.insert(pair<string, pair<Object*, Object*>>(objectPairId, objectPair));
                    }
                }
            }
        }
    return collisionMap;
}