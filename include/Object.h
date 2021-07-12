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
        Point acceleration;
        Matrix rotationMatrix;
        Point angularVelocity;
        double mass;
        Color color;
        double elasticityCoef;
        Matrix baseInertiaTensor;
        Point velocityForUpdate;
        Point angularVelocityForUpdate;
        bool isStatic;
        Point numVelocityUpdatesThisFramePerAxis;
        Point numAngularVelocityUpdatesThisFramePerAxis;
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
        
        double getElasticity();
        void setPos(Point);
        void setVel(Point);
        void updatePosAndVel(double);
        void queueVelocityUpdates(Point, Point);
        void applyVelocityUpdates();

        // Virtual methods
        virtual void draw() = 0;
        virtual Matrix getInertiaTensor();
};

#endif
