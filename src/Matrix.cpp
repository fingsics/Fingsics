#include "../include/Matrix.h"

using namespace std;

Matrix::Matrix(double* row0, double* row1, double* row2) {
    this->values[0][0] = row0[0];
    this->values[0][1] = row0[1];
    this->values[0][2] = row0[2];
    this->values[1][0] = row1[0];
    this->values[1][1] = row1[1];
    this->values[1][2] = row1[2];
    this->values[2][0] = row2[0];
    this->values[2][1] = row2[1];
    this->values[2][2] = row2[2];
}

Matrix::Matrix(Point row0, Point row1, Point row2) : Matrix::Matrix(row0.getX(), row0.getY(), row0.getZ(), row1.getX(), row1.getY(), row1.getZ(), row2.getX(), row2.getY(), row2.getZ()) {}

Matrix::Matrix(double v00, double v01, double v02, double v10, double v11, double v12, double v20, double v21, double v22) {
    this->values[0][0] = v00;
    this->values[0][1] = v01;
    this->values[0][2] = v02;
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

Matrix::Matrix(Point angles) {
    // TODO: Check rotation matrix
    Matrix x = Matrix(1, 0, 0,
                      0, cos(angles.getX()), -sin(angles.getX()),
                      0, sin(angles.getX()), cos(angles.getX()));

    Matrix y = Matrix(cos(angles.getY()), 0, sin(angles.getY()),
                      0, 1, 0,
                      -sin(angles.getY()), 0, cos(angles.getY()));

    Matrix z = Matrix(cos(angles.getZ()), -sin(angles.getZ()), 0,
                          sin(angles.getZ()), cos(angles.getZ()), 0,
                          0, 0, 1);

    Matrix res = z * y * x;

    this->values[0][0] = res[0][0];
    this->values[0][1] = res[0][1];
    this->values[0][2] = res[0][2];
    this->values[1][0] = res[1][0];
    this->values[1][1] = res[1][1];
    this->values[1][2] = res[1][2];
    this->values[2][0] = res[2][0];
    this->values[2][1] = res[2][1];
    this->values[2][2] = res[2][2];
}

GLdouble* Matrix::getOpenGLRotationMatrix() {
    GLdouble ret[16] = { values[0][0], values[1][0], values[2][0], 0,
                         values[0][1], values[1][1], values[2][1], 0,
                         values[0][2], values[1][2], values[2][2], 0,
                         0, 0, 0, 1 };
    return ret;

}

double Matrix::det() {
    return values[0][0] * (values[1][1] * values[2][2] - values[2][1] * values[1][2]) -
           values[0][1] * (values[1][0] * values[2][2] - values[1][2] * values[2][0]) +
           values[0][2] * (values[1][0] * values[2][1] - values[1][1] * values[2][0]);
}


Matrix Matrix::transpose() {
    return Matrix(values[0][0], values[1][0], values[2][0],
                    values[0][1], values[1][1], values[2][1],
                    values[0][2], values[1][2], values[2][2]);
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

Point Matrix::col0() {
    return Point(values[0][0], values[1][0], values[2][0]);
}

Point Matrix::col1() {
    return Point(values[0][1], values[1][1], values[2][1]);
}

Point Matrix::col2() {
    return Point(values[0][2], values[1][2], values[2][2]);
}
Matrix Matrix::operator*(double scalar) {
    return Matrix(row0() * scalar,
                  row1() * scalar,
                  row2() * scalar);
}

Point Matrix::operator*(Point vector) {
    return Point(vector.dotProduct(row0()),
                 vector.dotProduct(row1()), 
                 vector.dotProduct(row2()));
}

Matrix Matrix::operator*(Matrix other) {
    return Matrix(row0().dotProduct(other.col0()), row0().dotProduct(other.col1()), row0().dotProduct(other.col2()),
                    row1().dotProduct(other.col0()), row1().dotProduct(other.col1()), row1().dotProduct(other.col2()),
                    row2().dotProduct(other.col0()), row2().dotProduct(other.col1()), row2().dotProduct(other.col2()));
}

double* Matrix::operator[](int i) {
    return values[i];
}