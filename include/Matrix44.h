#ifndef MATRIX44_H
#define MATRIX44_H

#define _USE_MATH_DEFINES
#include "Point.h"
#include <math.h>

using namespace std;

class Matrix44 {
    private:
        double values[4][4];
        Matrix44 xAxisRotationMatrix(double);
        Matrix44 yAxisRotationMatrix(double);
        Matrix44 zAxisRotationMatrix(double);
    public:
        Matrix44(double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double);
        Matrix44(double[], double[], double[], double[]);
        Matrix44();
        Matrix44(Point); // Rotation matrix from rotation
        double* getOpenGLMatrix();
        Matrix44 operator*(Matrix44);
        Point operator*(Point);
        double* operator[](int);
};

#endif
