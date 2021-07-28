#ifndef OBJECT_H
#define OBJECT_H
#include "Color.h"
#include "Point.h"
#include "Matrix.h"
#include "OBB.h"
#include "AABB.h"
#include "Impulse.h"
#include <string>
#include <list>

using namespace std;

class Object {
    protected:
        string id;
        Point pos;
        Point vel;
        Point acceleration;
        Matrix rotationMatrix;
        Point angularVelocity;
        float mass;
        Color color;
        float elasticityCoef;
        Matrix baseInertiaTensor;
        Matrix invertedInertiaTensor;
        list<Impulse> queuedImpulses;
        Point velCollisionMassPerAxis;
        Point angVelCollisionMassPerAxis;
        bool isStatic;
        OBB obb;
        AABB* aabb;
        virtual void drawOBB();
        virtual void drawAABB();
        virtual void drawObject() = 0;
    public:
        Object(string, bool, Point, Point, Point, Point, Point, float, float, Color);
        float getMass();
        string getId();
        Point getPos();
        Point getVel();
        Point getAcceleration();
        Matrix getRotationMatrix();
        float* getOpenGLRotationMatrix();
        Point getAngularVelocity();
        bool getIsStatic();
        OBB getOBB();
        AABB* getAABB();
        
        float getElasticity();
        void setPos(Point);
        void setVel(Point);
        void setAngularVelocity(Point);
        virtual void setRotation(Matrix);
        void setAABB(AABB*);
        void updatePosAndVel(float);
        void queueImpulse(Point, Point, float, float);
        void applyQueuedImpulses();
        void applyImpulse(Point, Point);
        void draw(bool, bool);
        virtual Matrix getInertiaTensorInverse();
        virtual float getMinX() = 0;
        virtual float getMinY() = 0;
        virtual float getMinZ() = 0;
        virtual float getMaxX() = 0;
        virtual float getMaxY() = 0;
        virtual float getMaxZ() = 0;
};

#endif
