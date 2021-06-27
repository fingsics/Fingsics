#include "../include/Matrix.h"

using namespace std;

Matrix::Matrix(Point row1, Point row2, Point row3) {
    this->row1 = row1;
    this->row2 = row2;
    this->row3 = row3;
}

Matrix::Matrix() {
    this->row1 = Point();
    this->row2 = Point();
    this->row3 = Point();
}

double Matrix::det() {
    return row1.getX() * (row2.getY() * row3.getZ() - row3.getY() * row2.getZ()) -
        row1.getY() * (row2.getX() * row3.getZ() - row2.getZ() * row3.getX()) +
        row1.getZ() * (row2.getX() * row3.getY() - row2.getY() * row3.getX());
}

Matrix Matrix::inverse() {
    double invdet = 1 / det();

    return Matrix(Point((row2.getY() * row3.getZ() - row3.getY() * row2.getZ()) * invdet,
                        (row1.getZ() * row3.getY() - row1.getY() * row3.getZ()) * invdet,
                        (row1.getY() * row2.getZ() - row1.getZ() * row2.getY()) * invdet),
                  Point((row2.getZ() * row3.getX() - row2.getX() * row3.getZ()) * invdet,
                        (row1.getX() * row3.getZ() - row1.getZ() * row3.getX()) * invdet,
                        (row2.getX() * row1.getZ() - row1.getX() * row2.getZ()) * invdet),
                  Point((row2.getX() * row3.getY() - row3.getX() * row2.getY()) * invdet,
                        (row3.getX() * row1.getY() - row1.getX() * row3.getY()) * invdet,
                        (row1.getX() * row2.getY() - row2.getX() * row1.getY()) * invdet));
}

Point Matrix::operator*(Point vector) {
    return Point(vector.dotProduct(row1),
                 vector.dotProduct(row2), 
                 vector.dotProduct(row3));
}
