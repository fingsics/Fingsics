#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include "Color.h"
#include "Point.h"

using namespace std;

class Object {
    protected:
        string id;
        double posX;
        double posY;
        double posZ;
        double velX;
        double velY;
        double velZ;
        double mass;
        Color* color;
        void decreaseVelocity(double);
    public:
        Object(string, double, double, double, double, Color*);
        double getPosX();
        double getPosY();
        double getPosZ();
        double getMass();
        bool isMoving();
        string getId();
        Point* getPos();
        Point* getVel();
        void setPos(Point*);
        void updatePosAndVel(double);
        void setVelocity(Point*);

        // Virtual methods
        virtual void draw() = 0;
};

#endif
