#include "../include/NoBPCD.h"

// Adds every posible object pair to the map
map<string, pair<Object*, Object*>> NoBPCD::getCollisions(vector<Object*> objects) {
    map<string, pair<Object*, Object*>> collisionMap;
    for (int i = 0; i < objects.size(); i++) {
        for (int j = i + 1; j < objects.size(); j++) {
            if (objects[i]->getIsStatic() && objects[j]->getIsStatic()) continue;
            pair<string, pair<Object*, Object*>> objectPair = getObjectPairWithId(objects[i], objects[j]);
            if (collisionMap.find(objectPair.first) == collisionMap.end()) {
                collisionMap.insert(objectPair);
            }
        }
    }
    return collisionMap;
}
