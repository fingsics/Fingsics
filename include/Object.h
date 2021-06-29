#ifndef OBJECT_H
#define OBJECT_H
#include "Color.h"
#include "Point.h"
#include "Matrix33.h"
#include "Matrix44.h"
#include <string>

using namespace std;

class Object {
    protected:
        string id;
        Point pos;
        Point vel;
        Point force;
        Matrix44 rotationMatrix;
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
        Matrix44 getRotationMatrix();
        Point getAngularVelocity();
        
        double getElasticity();
        void setPos(Point);
        void setVel(Point);
        void setAngularVelocity(Point);
        void setForce(Point);
        void updatePosAndVel(double);

        // Virtual methods
        virtual void draw() = 0;
        virtual Matrix33 getInertiaTensor() = 0;
};

#endif
