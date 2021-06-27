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

bool Point::isZero() {
    return x == 0 && y == 0 && z == 0;
}

Point Point::rotate(Point angle) {
    // https://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space
    double radX = angle.getX() * M_PI / 180;
    double radY = angle.getY() * M_PI / 180;
    double radZ = angle.getZ() * M_PI / 180;
    // Rotate around X axis
    Point temp = Point(x, y * cos(radX) - z * sin(radX), y * sin(radX) + z * cos(radX));
    // Rotate around Y axis
    Point temp2 = Point(temp.getX() * cos(radY) + temp.getZ() * sin(radY), temp.getY(), -temp.getX() * sin(radY) + temp.getZ() * cos(radY));
    // Rotate around Z axis
    return Point(temp2.getX() * cos(radZ) - temp2.getY() * sin(radZ), temp2.getX() * sin(radZ) + temp2.getY() * cos(radZ), temp2.getZ());
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
