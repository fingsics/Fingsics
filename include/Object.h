#ifndef OBJECT_H
#define OBJECT_H
#include "Color.h"
#include "Point.h"
#include "Matrix.h"
#include "OBB.h"
#include <string>
#include <list>

using namespace std;

struct Impulse {
    Point normal;
    Point tangent;
    double magnitude;
    double mass;
    Impulse(Point, Point, double, double);
};

class Object {
    protected:
        string id;
        Point pos;
        Point vel;
        Point acceleration;
        Matrix rotationMatrix;
        Point angularVelocity;
        double mass;
        Color color;
        double elasticityCoef;
        Matrix baseInertiaTensor;
        Matrix invertedInertiaTensor;
        list<Impulse> queuedImpulses;
        Point velCollisionMassPerAxis;
        Point angVelCollisionMassPerAxis;
        bool isStatic;
        OBB obb;
        void drawOBB();
        virtual void drawObject() = 0;
    public:
        Object(string, bool, Point, Point, Point, Point, Point, double, double, Color);
        double getMass();
        bool isMoving();
        string getId();
        Point getPos();
        Point getVel();
        Point getAcceleration();
        Matrix getRotationMatrix();
        double* getOpenGLRotationMatrix();
        Point getAngularVelocity();
        bool getIsStatic();
        OBB getOBB();
        
        double getElasticity();
        void setPos(Point);
        void setVel(Point);
        void setAngularVelocity(Point);
        void setRotation(Matrix rotationMatrix);
        void updatePosAndVel(double);
        void queueImpulse(Point, Point, double, double);
        void applyQueuedImpulses();
        void applyImpulse(Point, Point);
        void draw(bool);
        virtual Matrix getInertiaTensorInverse();
};

#endif
