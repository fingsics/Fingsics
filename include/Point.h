#ifndef POINT_H
#define POINT_H

#define _USE_MATH_DEFINES
#include <math.h>
#include "Constants.h"

using namespace std;

class Point {
    private:
        double x;
        double y;
        double z;
    public:
        Point(double, double, double);
        Point();
        double getX();
        double getY();
        double getZ();
        double dotProduct(Point);
        Point crossProduct(Point);
        double getMagnitude();
        double getMagnitudeSqr();
        Point invert();
        Point normalize();
        bool isZero();
        bool isZero(double);
        Point addIfComponentNotZero(Point, double);
        Point rotate(Point);
        Point operator/(Point);
        Point operator-(Point);
        Point operator+(Point);
        Point operator/(double);
        Point operator*(double);
        Point operator*(Point);
        double operator[](int i);
        bool equals(Point);
        bool equals(Point, double);
        bool hasSameDirection(Point, double);
};

#endif
