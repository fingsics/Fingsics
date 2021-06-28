#ifndef MATRIX_H
#define MATRIX_H

#define _USE_MATH_DEFINES
#include "Point.h"
#include <math.h>

using namespace std;

class Matrix {
    private:
        double values[3][3];
    public:
        Matrix(double, double, double, double, double, double, double, double, double);
        Matrix(double[], double[], double[]);
        Matrix();
        double det();
        Matrix transpose();
        Matrix inverse();
        Point row0();
        Point row1();
        Point row2();
        Point col0();
        Point col1();
        Point col2();
        Point operator*(Point);
        Matrix operator*(Matrix);
};

#endif
