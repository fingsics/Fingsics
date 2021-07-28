#ifndef COLISSION_H
#define COLISSION_H

#include "Object.h"

using namespace std;

struct Collision {
    private:
        Object* object1;
        Object* object2;
        Point point;
        Point normal;
        float penetrationDepth;
        float lastPenetrationDepth;
    public:
        Collision(Object*, Object*, Point, Point, float, float);
        Collision(Point, Point, float);
        Object* getObject1();
        Object* getObject2();
        Point getPoint();
        Point getNormal();
        float getPenetrationDepth();
        float getLastPenetrationDepth();
        void setObjects(pair<Object*, Object*>);
        void setLastPenetrationDepth(float);
        void invertNormal();
};

#endif
