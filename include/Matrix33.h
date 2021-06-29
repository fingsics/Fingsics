#ifndef MATRIX33_H
#define MATRIX33_H

#define _USE_MATH_DEFINES
#include "Point.h"
#include <math.h>

using namespace std;

class Matrix33 {
    private:
        double values[3][3];
    public:
        Matrix33(double, double, double, double, double, double, double, double, double);
        Matrix33(double[], double[], double[]);
        Matrix33();
        double det();
        Matrix33 transpose();
        Matrix33 inverse();
        Point row0();
        Point row1();
        Point row2();
        Point col0();
        Point col1();
        Point col2();
        Point operator*(Point);
        Matrix33 operator*(Matrix33);
};

#endif
