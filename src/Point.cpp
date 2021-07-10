#include "../include/Point.h"

using namespace std;

Point::Point(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Point::Point() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

double Point::getX() {
  return x;
}

double Point::getY() {
  return y;
}

double Point::getZ() {
  return z;
}

Point Point::normalize() {
    return (*this) / getMagnitude();
}

double Point::getMagnitude(){
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

double Point::getSquareMagnitude() {
    return pow(x, 2) + pow(y, 2) + pow(z, 2);
}

bool Point::isZero() {
    return x == 0 && y == 0 && z == 0;
}

Point Point::rotate(Point angleInDegrees) {
    Point angle = angleInDegrees * M_PI / 180;
    // https://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space
    // Rotate around X axis
    Point temp = Point(x, y * cos(angle.getX()) - z * sin(angle.getX()), y * sin(angle.getX()) + z * cos(angle.getX()));
    // Rotate around Z axis
    Point temp2 = Point(temp.getX() * cos(angle.getZ()) - temp.getY() * sin(angle.getZ()), temp.getX() * sin(angle.getZ()) + temp.getY() * cos(angle.getZ()), temp.getZ());
    // Rotate around Y axis
    return Point(temp2.getX() * cos(angle.getY()) + temp2.getZ() * sin(angle.getY()), temp2.getY(), -temp2.getX() * sin(angle.getY()) + temp2.getZ() * cos(angle.getY())); 
}

double Point::dotProduct(Point vector2){
    return (x * vector2.getX()) + (y * vector2.getY()) + (z * vector2.getZ());
}

Point Point::crossProduct(Point vector2){
    return Point(
        y * vector2.getZ() - z * vector2.getY(),
        z * vector2.getX() - x * vector2.getZ(),
        x * vector2.getY() - y * vector2.getX()
    );
}

double Point::distanceTo(Point other) {
    Point diff = *this - other;
    return diff.getMagnitude();
}

Point Point::operator-(Point vector2){
    return Point(x - vector2.getX(), y - vector2.getY(), z - vector2.getZ());
}

Point Point::operator+(Point vector2){
    return Point(x + vector2.getX(), y + vector2.getY(), z + vector2.getZ());
}

Point Point::operator/(double divisor){
    return Point(x / divisor, y / divisor, z / divisor);
}

Point Point::operator*(double divisor){
    return Point(x * divisor, y * divisor, z * divisor);
}

double Point::operator[](int i) {
    if (i == 0) return x;
    else if (i == 1) return y;
    else if (i == 2) return z;
    throw "Invalid point index.";
}

bool Point::equals(Point other) {
    Point diff = *this - other;
    return diff.getSquareMagnitude() < 0.00001;
}