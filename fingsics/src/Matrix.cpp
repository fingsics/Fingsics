#include "../include/Matrix.h"

Matrix::Matrix(Vector row0, Vector row1, Vector row2) : Matrix::Matrix(row0.getX(), row0.getY(), row0.getZ(), row1.getX(), row1.getY(), row1.getZ(), row2.getX(), row2.getY(), row2.getZ()) {}

Matrix::Matrix(float v00, float v01, float v02, float v10, float v11, float v12, float v20, float v21, float v22) {
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

Matrix::Matrix(Vector angles) {
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

GLfloat* Matrix::getOpenGLRotationMatrix() {
    GLfloat ret[16] = { values[0][0], values[1][0], values[2][0], 0,
                         values[0][1], values[1][1], values[2][1], 0,
                         values[0][2], values[1][2], values[2][2], 0,
                         0, 0, 0, 1 };
    return ret;

}

float Matrix::det() {
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
    float invdet = 1 / det();

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

Vector Matrix::row(int i) {
    switch (i) {
    case 0:
        return Vector(values[0][0], values[0][1], values[0][2]);
    case 1:
        return Vector(values[1][0], values[1][1], values[1][2]);
    case 2:
        return Vector(values[2][0], values[2][1], values[2][2]);
    default:
        throw std::runtime_error("Invalid row index");
    }
}

Vector Matrix::col(int i) {
    switch (i) {
    case 0:
        return Vector(values[0][0], values[1][0], values[2][0]);
    case 1:
        return Vector(values[0][1], values[1][1], values[2][1]);
    case 2:
        return Vector(values[0][2], values[1][2], values[2][2]);
    default:
        throw std::runtime_error("Invalid column index");
    }
}

void Matrix::set(int row, int col, float value) {
    values[row][col] = value;
}

Matrix Matrix::operator*(float scalar) {
    return Matrix(values[0][0] * scalar, values[0][1] * scalar, values[0][2] * scalar,
                  values[1][0] * scalar, values[1][1] * scalar, values[1][2] * scalar,
                  values[2][0] * scalar, values[2][1] * scalar, values[2][2] * scalar);
                  
}

Matrix Matrix::operator/(float scalar) {
    return Matrix(values[0][0] / scalar, values[0][1] / scalar, values[0][2] / scalar,
                  values[1][0] / scalar, values[1][1] / scalar, values[1][2] / scalar,
                  values[2][0] / scalar, values[2][1] / scalar, values[2][2] / scalar);

}

Vector Matrix::operator*(Vector vector) {
    return Vector(vector.dotProduct(row(0)),
                 vector.dotProduct(row(1)), 
                 vector.dotProduct(row(2)));
}

Matrix Matrix::operator*(Matrix other) {
    return Matrix(row(0).dotProduct(other.col(0)), row(0).dotProduct(other.col(1)), row(0).dotProduct(other.col(2)),
                    row(1).dotProduct(other.col(0)), row(1).dotProduct(other.col(1)), row(1).dotProduct(other.col(2)),
                    row(2).dotProduct(other.col(0)), row(2).dotProduct(other.col(1)), row(2).dotProduct(other.col(2)));
}

float* Matrix::operator[](int i) {
    return values[i];
}
