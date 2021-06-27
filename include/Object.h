#ifndef OBJECT_H
#define OBJECT_H
#include "Color.h"
#include "Point.h"
#include "Matrix.h"
#include <string>

using namespace std;

class Object {
    protected:
        string id;
        Point pos;
        Point vel;
        Point force;
        Point angle;
        Point angularVelocity;
        double mass;
        Color color;
        double elasticityCoef;
    public:
        Object(string, Point, Point, Point, Point, Point, double, double, Color);
        double getMass();
        bool isMoving();
        string getId();
        Point getPos();
        Point getVel();
        Point getForce();
        Point getAngle();
        Point getAngularVelocity();
        
        double getElasticity();
        void setPos(Point);
        void setVel(Point);
        void setAngularVelocity(Point);
        void setForce(Point);

        // Virtual methods
        virtual void draw() = 0;
        virtual void updatePosAndVel(double) = 0;
        virtual Matrix getInertiaTensor() = 0;
};

#endif
