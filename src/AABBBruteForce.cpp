#include "../include/AABBBruteForce.h"

map<string, pair<Object*, Object*>> AABBBruteForce::getCollisions(vector<Object*> objects) {
    map<string, pair<Object*, Object*>> collisionMap;
    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {
            if (objects[i]->getIsStatic() && objects[j]->getIsStatic()) continue;
            pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(objects[i], objects[j]);
            if (collisionMap.find(objectPair.first) == collisionMap.end() && AABBOverlapTest(objects[i], objects[j])) {
                collisionMap.insert(objectPair);
            }
        }
    }
    return collisionMap;
}

bool AABBBruteForce::AABBOverlapTest(Object* object1, Object* object2) {
    if (object1->getMinX() > object2->getMaxX() || object2->getMinX() > object1->getMaxX()) return false;
    if (object1->getMinY() > object2->getMaxY() || object2->getMinY() > object1->getMaxY()) return false;
    if (object1->getMinZ() > object2->getMaxZ() || object2->getMinZ() > object1->getMaxZ()) return false;
    return true;
}
