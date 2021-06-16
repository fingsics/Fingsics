#ifndef POINT_H
#define POINT_H

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
        double magnitude();
        bool isZero();
        Point operator-(Point);
        Point operator+(Point);
        Point operator/(double);
        Point operator*(double);
};

#endif
