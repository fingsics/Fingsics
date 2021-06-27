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
    // TODO
    return 0;
}

Matrix Matrix::inverse() {
    // TODO
    Point invRow1 = Point(0, 0, 0);
    Point invRow2 = Point(0, 0, 0);
    Point invRow3 = Point(0, 0, 0);

    return Matrix(invRow1, invRow2, invRow3);
}

Point Matrix::operator*(Point vector) {
    // TODO: multiplicacion matriz x vector
    return Point();
}
