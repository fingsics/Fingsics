#include "../include/Helpers.h"

string getObjectPairId(pair<Object*, Object*> objectPair) {
    string firstKey = objectPair.first->getId() < objectPair.second->getId() ? objectPair.first->getId() : objectPair.second->getId();
    string secondKey = objectPair.first->getId() < objectPair.second->getId() ? objectPair.second->getId() : objectPair.first->getId();
    return firstKey + "-" + secondKey;
}