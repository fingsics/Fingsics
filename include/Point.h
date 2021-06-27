#ifndef POINT_H
#define POINT_H

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

class Point{
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
        double distanceTo(Point);
        double magnitude();
        bool isZero();
        Point rotate(Point);
        Point operator-(Point);
        Point operator+(Point);
        Point operator/(double);
        Point operator*(double);
};

#endif
