#ifndef COLISSION_H
#define COLISSION_H

#include "Object.h"

using namespace std;

struct Collision {
    private:
        Object* object1;
        Object* object2;
        Vector point;
        Vector normal;
        float penetrationDepth;
        float lastPenetrationDepth;
        float secondToLastPenetrationDepth;
    public:
        Collision(Vector, Vector, float);
        Object* getObject1();
        Object* getObject2();
        Vector getPoint();
        Vector getNormal();
        float getPenetrationDepth();
        float getLastPenetrationDepth();
        void setObjects(pair<Object*, Object*>);
        void setLastPenetrationDepth(float);
        void setSecondToLastPenetrationDepth(float);
        void invertNormal();
        bool shouldApplyIt();
};

#endif
