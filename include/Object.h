#ifndef OBJECT_H
#define OBJECT_H
#include "Color.h"
#include "Point.h"
#include "Matrix.h"
#include "OBB.h"
#include "AABB.h"
#include "Impulse.h"
#include <string>
#include <vector>
#include <list>

using namespace std;

class Object {
    protected:
        string id;
        Point position;
        Point velocity;
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

        void applyImpulse(Point, Point);

        virtual void drawOBB();
        virtual void drawAABB();
        virtual void drawObject() = 0;

        // Draw only
        bool replayMode;
        Point* positions;
        Matrix* rotationMatrices;
        int frames;
    public:
        Object(string, bool, Point, Point, Point, Point, Point, float, float, Color);
        Object(string, Color, Point*, Matrix*, int);
        float getMass();
        string getId();
        Point getPosition();
        Point getVelocity();
        Point getAcceleration();
        Matrix getRotationMatrix();
        Point getAngularVelocity();
        bool getIsStatic();
        OBB getOBB();
        AABB* getAABB();
        Color getColor();
        
        float getElasticity();
        void setPosition(Point);
        void setVelocity(Point);
        void setAngularVelocity(Point);
        virtual void setRotation(Matrix);
        void setAABB(AABB*);
        void updatePositionAndVelocity(float);
        void goToFrame(int);
        void queueImpulse(Point, Point, float, float);
        void applyQueuedImpulses();
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
