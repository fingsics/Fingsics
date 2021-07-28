#ifndef MATRIX33_H
#define MATRIX33_H

#define _USE_MATH_DEFINES
#include "Point.h"
#include "SDL_opengl.h"
#include <math.h>

using namespace std;

class Matrix {
    private:
        float values[3][3];
    public:
        Matrix(float, float, float, float, float, float, float, float, float);
        Matrix(Point, Point, Point);
        Matrix(Point);
        Matrix();
        float det();
        Matrix transpose();
        Matrix inverse();
        GLfloat* getOpenGLRotationMatrix();
        Point col(int);
        Point row(int);
        Matrix operator*(float);
        Matrix operator/(float);
        Point operator*(Point);
        Matrix operator*(Matrix);
        float* operator[](int);
};

#endif
