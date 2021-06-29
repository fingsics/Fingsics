#include "../include/Matrix44.h"

using namespace std;

Matrix44::Matrix44(double* row1, double* row2, double* row3, double* row4) {
    this->values[0][0] = row1[0];
    this->values[0][1] = row1[1];
    this->values[0][2] = row1[2];
    this->values[0][3] = row1[3];
    this->values[1][0] = row2[0];
    this->values[1][1] = row2[1];
    this->values[1][2] = row2[2];
    this->values[1][3] = row2[3];
    this->values[2][0] = row3[0];
    this->values[2][1] = row3[1];
    this->values[2][2] = row3[2];
    this->values[2][3] = row3[3];
    this->values[3][0] = row4[0];
    this->values[3][1] = row4[1];
    this->values[3][2] = row4[2];
    this->values[3][3] = row4[3];
}

Matrix44::Matrix44(double v00, double v01, double v02, double v03,
                   double v10, double v11, double v12, double v13,
                   double v20, double v21, double v22, double v23,
                   double v30, double v31, double v32, double v33) {
    this->values[0][0] = v00;
    this->values[0][1] = v01;
    this->values[0][2] = v02;
    this->values[0][3] = v03;
    this->values[1][0] = v10;
    this->values[1][1] = v11;
    this->values[1][2] = v12;
    this->values[1][3] = v13;
    this->values[2][0] = v20;
    this->values[2][1] = v21;
    this->values[2][2] = v22;
    this->values[2][3] = v23;
    this->values[3][0] = v30;
    this->values[3][1] = v31;
    this->values[3][2] = v32;
    this->values[3][3] = v33;
}

Matrix44::Matrix44() {
    this->values[0][0] = 1;
    this->values[0][1] = 0;
    this->values[0][2] = 0;
    this->values[0][3] = 0;
    this->values[1][0] = 0;
    this->values[1][1] = 1;
    this->values[1][2] = 0;
    this->values[1][3] = 0;
    this->values[2][0] = 0;
    this->values[2][1] = 0;
    this->values[2][2] = 1;
    this->values[2][3] = 0;
    this->values[3][0] = 0;
    this->values[3][1] = 0;
    this->values[3][2] = 0;
    this->values[3][3] = 1;
}

Matrix44::Matrix44(Point angles) {
    // TODO: Get actual rotation matrix
    Matrix44 x = Matrix44(1, 0, 0, 0,
        0, cos(angles.getX()), -sin(angles.getX()), 0,
        0, sin(angles.getX()), cos(angles.getX()), 0,
        0, 0, 0, 1);

    Matrix44 y = Matrix44(cos(angles.getY()), 0, sin(angles.getY()), 0,
        0, 1, 0, 0,
        -sin(angles.getY()), 0, cos(angles.getY()), 0,
        0, 0, 0, 1);

    Matrix44 z = Matrix44(cos(angles.getZ()), -sin(angles.getZ()), 0, 0,
        sin(angles.getZ()), cos(angles.getZ()), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);

    Matrix44 res = x * y * z;

    this->values[0][0] = res[0][0];
    this->values[0][1] = res[0][1];
    this->values[0][2] = res[0][2];
    this->values[0][3] = res[0][3];
    this->values[1][0] = res[1][0];
    this->values[1][1] = res[1][1];
    this->values[1][2] = res[1][2];
    this->values[1][3] = res[1][3];
    this->values[2][0] = res[2][0];
    this->values[2][1] = res[2][1];
    this->values[2][2] = res[2][2];
    this->values[2][3] = res[2][3];
    this->values[3][0] = res[3][0];
    this->values[3][1] = res[3][1];
    this->values[3][2] = res[3][2];
    this->values[3][3] = res[3][3];
}

double* Matrix44::getOpenGLMatrix() {
    return new double[values[0][0], values[0][1], values[0][2], values[0][3],
                      values[1][0], values[1][1], values[1][2], values[1][3],
                      values[2][0], values[2][1], values[2][2], values[2][3],
                      values[3][0], values[3][1], values[3][2], values[3][3]];
}

double* Matrix44::operator[](int i) {
    return values[i];
}

Point Matrix44::operator*(Point point) {
    return Point(point.getX() * values[0][0] + point.getY() * values[0][1] + point.getZ() * values[0][2],
                 point.getX() * values[1][0] + point.getY() * values[1][1] + point.getZ() * values[1][2],
                 point.getX() * values[2][0] + point.getY() * values[2][1] + point.getZ() * values[2][2]);
}

Matrix44 Matrix44::operator*(Matrix44 other) {
    return Matrix44(values[0][0] * other[0][0] + values[1][0] * other[0][1] + values[2][0] * other[0][2] + values[3][0] * other[0][3],
                    values[0][1] * other[0][0] + values[1][1] * other[0][1] + values[2][1] * other[0][2] + values[3][1] * other[0][3],
                    values[0][2] * other[0][0] + values[1][2] * other[0][1] + values[2][2] * other[0][2] + values[3][2] * other[0][3],
                    values[0][3] * other[0][0] + values[1][3] * other[0][1] + values[2][3] * other[0][2] + values[3][3] * other[0][3],

                    values[0][0] * other[1][0] + values[1][0] * other[1][1] + values[2][0] * other[1][2] + values[3][0] * other[1][3],
                    values[0][1] * other[1][0] + values[1][1] * other[1][1] + values[2][1] * other[1][2] + values[3][1] * other[1][3],
                    values[0][2] * other[1][0] + values[1][2] * other[1][1] + values[2][2] * other[1][2] + values[3][2] * other[1][3],
                    values[0][3] * other[1][0] + values[1][3] * other[1][1] + values[2][3] * other[1][2] + values[3][3] * other[1][3],

                    values[0][0] * other[2][0] + values[1][0] * other[2][1] + values[2][0] * other[2][2] + values[3][0] * other[2][3],
                    values[0][1] * other[2][0] + values[1][1] * other[2][1] + values[2][1] * other[2][2] + values[3][1] * other[2][3],
                    values[0][2] * other[2][0] + values[1][2] * other[2][1] + values[2][2] * other[2][2] + values[3][2] * other[2][3],
                    values[0][3] * other[2][0] + values[1][3] * other[2][1] + values[2][3] * other[2][2] + values[3][3] * other[2][3],

                    values[0][0] * other[3][0] + values[1][0] * other[3][1] + values[2][0] * other[3][2] + values[3][0] * other[3][3],
                    values[0][1] * other[3][0] + values[1][1] * other[3][1] + values[2][1] * other[3][2] + values[3][1] * other[3][3],
                    values[0][2] * other[3][0] + values[1][2] * other[3][1] + values[2][2] * other[3][2] + values[3][2] * other[3][3],
                    values[0][3] * other[3][0] + values[1][3] * other[3][1] + values[2][3] * other[3][2] + values[3][3] * other[3][3]);
}