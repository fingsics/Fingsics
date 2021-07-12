#ifndef POINT_H
#define POINT_H

#define _USE_MATH_DEFINES
#include <math.h>

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
        Point rotate(Point);
        Point operator-(Point);
        Point operator+(Point);
        Point operator/(double);
        Point operator*(double);
        double* toArray();
        double operator[](int i);
        bool equals(Point);
};

#endif
