#ifndef MATRIX33_H
#define MATRIX33_H

#define _USE_MATH_DEFINES
#include "Point.h"
#include "SDL_opengl.h"
#include <math.h>

using namespace std;

class Matrix {
    private:
        double values[3][3];
    public:
        Matrix(double, double, double, double, double, double, double, double, double);
        Matrix(double[], double[], double[]);
        Matrix(Point, Point, Point);
        Matrix(Point);
        Matrix();
        double det();
        Matrix transpose();
        Matrix inverse();
        GLdouble* getOpenGLRotationMatrix();
        Point row0();
        Point row1();
        Point row2();
        Point col0();
        Point col1();
        Point col2();
        Matrix operator*(double);
        Matrix operator/(double);
        Point operator*(Point);
        Matrix operator*(Matrix);
        double* operator[](int);
        double** toArrays();
};

#endif
