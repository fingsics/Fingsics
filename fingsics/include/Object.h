#ifndef OBJECT_H
#define OBJECT_H
#include "Color.h"
#include "Vector.h"
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
        Vector position;
        Vector velocity;
        Vector acceleration;
        Matrix rotationMatrix;
        Vector angularVelocity;
        float mass;
        Color color;
        float elasticityCoef;
        Matrix baseInertiaTensor;
        Matrix invertedInertiaTensor;
        list<Impulse> queuedImpulses;
        Vector velCollisionMassPerAxis;
        Vector angVelCollisionMassPerAxis;
        bool isStatic;
        OBB obb;
        AABB* aabb;
        bool draw;

        void applyImpulse(Vector, Vector);

        // Draw only
        bool replayMode;
        Vector* positions;
        Matrix* rotationMatrices;
        int frames;
    public:
        Object(string, bool, Vector, Vector, Vector, Vector, Vector, float, float, Color, bool);
        Object(string, Color, Vector*, Matrix*, int, bool);
        float getMass();
        string getId();
        Vector getPosition();
        Vector getVelocity();
        Vector getAcceleration();
        Matrix getRotationMatrix();
        Vector getAngularVelocity();
        bool getIsStatic();
        OBB getOBB();
        AABB* getAABB();
        Color getColor();
        
        float getElasticity();
        void setPosition(Vector);
        void setVelocity(Vector);
        void setAngularVelocity(Vector);
        virtual void setRotation(Matrix);
        void setAABB(AABB*);
        void updatePositionAndVelocity(float);
        void goToFrame(int);
        void queueImpulse(Vector, Vector, float, float);
        void applyQueuedImpulses();
        virtual Matrix getInertiaTensorInverse();
        virtual float getMinX() = 0;
        virtual float getMinY() = 0;
        virtual float getMinZ() = 0;
        virtual float getMaxX() = 0;
        virtual float getMaxY() = 0;
        virtual float getMaxZ() = 0;
        bool getDraw();
};

#endif
