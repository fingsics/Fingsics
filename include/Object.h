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
        Matrix rotationMatrix;
        Point angularVelocity;
        double mass;
        Color color;
        double elasticityCoef;
        Matrix baseInertiaTensor;
        Point velocityForUpdate;
        Point angularVelocityForUpdate;
    public:
        Object(string, Point, Point, Point, Point, Point, double, double, Color);
        double getMass();
        bool isMoving();
        string getId();
        Point getPos();
        Point getVel();
        Point getForce();
        Matrix getRotationMatrix();
        double* getOpenGLRotationMatrix();
        Point getAngularVelocity();
        
        double getElasticity();
        void setPos(Point);
        void setForce(Point);
        void updatePosAndVel(double);
        void queueVelocityUpdates(Point, Point);
        void applyVelocityUpdates();

        // Virtual methods
        virtual void draw() = 0;
        virtual Matrix getInertiaTensor();
};

#endif
