#ifndef POINT_H
#define POINT_H

#define _USE_MATH_DEFINES
#include "Constants.h"
#include <math.h>
#include <stdexcept>

using namespace std;

class Vector {
    private:
        float x;
        float y;
        float z;
    public:
        Vector(float, float, float);
        Vector();
        float getX();
        float getY();
        float getZ();
        void setX(float);
        void setY(float);
        void setZ(float);
        float dotProduct(Vector);
        Vector crossProduct(Vector);
        float getMagnitude();
        float getMagnitudeSqr();
        Vector invert();
        Vector normalize();
        Vector roundToZeroIfNear();
        Vector abs();
        bool isZero();
        bool isZero(float);
        Vector addIfComponentNotZero(Vector, float);
        Vector rotate(Vector);
        Vector operator/(Vector);
        Vector operator-(Vector);
        Vector operator+(Vector);
        Vector operator/(float);
        Vector operator*(float);
        Vector operator*(Vector);
        float operator[](int i);
        bool equals(Vector);
        bool parallel(Vector);
        bool equals(Vector, float);
        bool hasSameDirection(Vector, float);
};

#endif
