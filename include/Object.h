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
        double elasticityCoef;
    public:
        Object(string, Point, Point, Point, double, double, Color);
        double getMass();
        bool isMoving();
        string getId();
        Point getPos();
        Point getVel();
        Point getForce();
        double getElasticity();
        void setPos(Point);
        void setVel(Point);
        void setForce(Point);
        virtual void updatePosAndVel(double) = 0;

        // Virtual methods
        virtual void draw() = 0;
};

#endif
