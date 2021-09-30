#include "../include/NoBPCD.h"

// Adds every posible object pair to the map
map<string, pair<Object*, Object*>> NoBPCD::getCollisions(Object** objects, int numObjects) {
    map<string, pair<Object*, Object*>> collisionMap;
    for (int i = 0; i < numObjects; i++) {
        for (int j = i + 1; j < numObjects; j++) {
            if (objects[i]->getIsStatic() && objects[j]->getIsStatic()) continue;
            pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(objects[i], objects[j]);
            if (collisionMap.find(objectPair.first) == collisionMap.end()) {
                collisionMap.insert(objectPair);
            }
        }
    }
    return collisionMap;
}
