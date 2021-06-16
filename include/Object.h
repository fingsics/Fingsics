#ifndef OBJECT_H
#define OBJECT_H
#include "Color.h"
#include "Point.h"
#include <string>

using namespace std;

class Object {
    protected:
        string id;
        Point pos;
        Point vel;
        double mass;
        Color color;
        void decreaseVelocity(double);
    public:
        Object(string, Point, Point, double, Color);
        double getMass();
        bool isMoving();
        string getId();
        Point getPos();
        Point getVel();
        void setPos(Point);
        void setVel(Point);
        void updatePosAndVel(double);

        // Virtual methods
        virtual void draw() = 0;
};

#endif
