#ifndef MATRIX_H
#define MATRIX_H

#define _USE_MATH_DEFINES
#include "Point.h"
#include <math.h>

using namespace std;

class Matrix {
    private:
        Point row1;
        Point row2;
        Point row3;
    public:
        Matrix(Point, Point, Point);
        Matrix();
        double det();
        Matrix inverse();
        Point operator*(Point);
};

#endif
