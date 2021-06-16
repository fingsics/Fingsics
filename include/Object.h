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
        Point force;
        double mass;
        Color color;
    public:
        Object(string, Point, Point, Point, double, Color);
        double getMass();
        bool isMoving();
        string getId();
        Point getPos();
        Point getVel();
        Point getForce();
        void setPos(Point);
        void setVel(Point);
        void setForce(Point);
        void updatePosAndVel(double);

        // Virtual methods
        virtual void draw() = 0;
};

#endif
