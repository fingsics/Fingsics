#ifndef MATRIX33_H
#define MATRIX33_H

#define _USE_MATH_DEFINES
#include "Vector.h"
#include "SDL_opengl.h"
#include <math.h>
#include <stdexcept>

using namespace std;

class Matrix {
    private:
        float values[3][3];
    public:
        Matrix(float, float, float, float, float, float, float, float, float);
        Matrix(Vector, Vector, Vector);
        Matrix(Vector);
        Matrix();
        float det();
        Matrix transpose();
        Matrix inverse();
        GLfloat* getOpenGLRotationMatrix();
        Vector col(int);
        Vector row(int);
        void set(int, int, float);
        Matrix operator*(float);
        Matrix operator/(float);
        Vector operator*(Vector);
        Matrix operator*(Matrix);
        float* operator[](int);
};

#endif
