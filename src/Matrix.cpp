#include "../include/Matrix.h"

using namespace std;

Matrix::Matrix(double* row1, double* row2, double* row3) {
    this->values[0][0] = row1[0];
    this->values[0][1] = row1[1];
    this->values[0][2] = row1[2];
    this->values[1][0] = row2[0];
    this->values[1][1] = row2[1];
    this->values[1][2] = row2[2];
    this->values[2][0] = row3[0];
    this->values[2][1] = row3[1];
    this->values[2][2] = row3[2];
}

Matrix::Matrix(double v01, double v02, double v03, double v10, double v11, double v12, double v20, double v21, double v22) {
    this->values[0][0] = v01;
    this->values[0][1] = v02;
    this->values[0][2] = v03;
    this->values[1][0] = v10;
    this->values[1][1] = v11;
    this->values[1][2] = v12;
    this->values[2][0] = v20;
    this->values[2][1] = v21;
    this->values[2][2] = v22;
}

Matrix::Matrix() {
    this->values[0][0] = 0;
    this->values[0][1] = 0;
    this->values[0][2] = 0;
    this->values[1][0] = 0;
    this->values[1][1] = 0;
    this->values[1][2] = 0;
    this->values[2][0] = 0;
    this->values[2][1] = 0;
    this->values[2][2] = 0;
}

double Matrix::det() {
    return values[0][0] * (values[1][1] * values[2][2] - values[2][1] * values[1][2]) -
           values[0][1] * (values[1][0] * values[2][2] - values[1][2] * values[2][0]) +
           values[0][2] * (values[1][0] * values[2][1] - values[1][1] * values[2][0]);
}

Matrix Matrix::inverse() {
    double invdet = 1 / det();

    return Matrix((values[1][1] * values[2][2] - values[2][1] * values[1][2]) * invdet,
                  (values[0][2] * values[2][1] - values[0][1] * values[2][2]) * invdet,
                  (values[0][1] * values[1][2] - values[0][2] * values[1][1]) * invdet,
                  (values[1][2] * values[2][0] - values[1][0] * values[2][2]) * invdet,
                  (values[0][0] * values[2][2] - values[0][2] * values[2][0]) * invdet,
                  (values[1][0] * values[0][2] - values[0][0] * values[1][2]) * invdet,
                  (values[1][0] * values[2][1] - values[2][0] * values[1][1]) * invdet,
                  (values[2][0] * values[0][1] - values[0][0] * values[2][1]) * invdet,
                  (values[0][0] * values[1][1] - values[1][0] * values[0][1]) * invdet);
}

Point Matrix::row0() {
    return Point(values[0][0], values[0][1], values[0][2]);
}

Point Matrix::row1() {
    return Point(values[1][0], values[1][1], values[1][2]);
}

Point Matrix::row2() {
    return Point(values[2][0], values[2][1], values[2][2]);
}

Point Matrix::operator*(Point vector) {
    return Point(vector.dotProduct(row0()),
                 vector.dotProduct(row1()), 
                 vector.dotProduct(row2()));
}
