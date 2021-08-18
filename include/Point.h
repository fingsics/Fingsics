#ifndef POINT_H
#define POINT_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "Constants.h"
#include <stdexcept>

using namespace std;

class Point {
    private:
        float x;
        float y;
        float z;
    public:
        Point(float, float, float);
        Point();
        float getX();
        float getY();
        float getZ();
        float dotProduct(Point);
        Point crossProduct(Point);
        float getMagnitude();
        float getMagnitudeSqr();
        Point invert();
        Point normalize();
        Point roundToZeroIfNear();
        Point abs();
        bool isZero();
        bool isZero(float);
        Point addIfComponentNotZero(Point, float);
        Point rotate(Point);
        Point operator/(Point);
        Point operator-(Point);
        Point operator+(Point);
        Point operator/(float);
        Point operator*(float);
        Point operator*(Point);
        float operator[](int i);
        bool equals(Point);
        bool parallel(Point);
        bool equals(Point, float);
        bool hasSameDirection(Point, float);
};

#endif
