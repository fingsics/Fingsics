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
        double penetrationDepth;
        double lastPenetrationDepth;
    public:
        Collision(Object*, Object*, Point, Point, double, double);
        Collision(Point, Point, double);
        Object* getObject1();
        Object* getObject2();
        Point getPoint();
        Point getNormal();
        double getPenetrationDepth();
        double getLastPenetrationDepth();
        void setObjects(pair<Object*, Object*>);
        void setLastPenetrationDepth(double);
        void invertNormal();
};

#endif
