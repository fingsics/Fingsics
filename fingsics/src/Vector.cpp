#include "../include/Vector.h"

using namespace std;

Vector::Vector(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector::Vector() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

float Vector::getX() {
  return x;
}

float Vector::getY() {
  return y;
}

float Vector::getZ() {
  return z;
}

void Vector::setX(float value) {
  x = value;
}

void Vector::setY(float value) {
  y = value;
}

void Vector::setZ(float value) {
  z = value;
}

Vector Vector::normalize() {
    float magnitude = getMagnitude();
    if (magnitude == 0) throw std::runtime_error("Can't normalize a null vector");
    return (*this) / magnitude;
}

Vector Vector::roundToZeroIfNear() {
    return Vector(fabs(x) < EPSILON ? 0 : x, fabs(y) < EPSILON ? 0 : y, fabs(z) < EPSILON ? 0 : z);
}

Vector Vector::invert() {
    return (*this) * -1;
}

Vector Vector::abs() {
    return Vector(fabs(x), fabs(y), fabs(z));
}

float Vector::getMagnitude() {
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

float Vector::getMagnitudeSqr() {
    return pow(x, 2) + pow(y, 2) + pow(z, 2);
}

bool Vector::isZero() {
    return fabs(x) < EPSILON && fabs(y) < EPSILON && fabs(z) < EPSILON;
}

bool Vector::isZero(float tolerance) {
    return fabs(x) < tolerance && fabs(y) < tolerance && fabs(z) < tolerance;
}

Vector Vector::addIfComponentNotZero(Vector vector, float value) {
    float retX = x;
    float retY = y;
    float retZ = z;
    if (fabs(vector.getX()) > EPSILON) retX += value;
    if (fabs(vector.getY()) > EPSILON) retY += value;
    if (fabs(vector.getZ()) > EPSILON) retZ += value;

    return Vector(retX, retY, retZ);
}

Vector Vector::rotate(Vector angleInDegrees) {
    Vector angle = angleInDegrees * M_PI / 180;
    // https://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space
    // Rotate around X axis
    Vector temp = Vector(x, y * cos(angle.getX()) - z * sin(angle.getX()), y * sin(angle.getX()) + z * cos(angle.getX()));
    // Rotate around Z axis
    Vector temp2 = Vector(temp.getX() * cos(angle.getZ()) - temp.getY() * sin(angle.getZ()), temp.getX() * sin(angle.getZ()) + temp.getY() * cos(angle.getZ()), temp.getZ());
    // Rotate around Y axis
    return Vector(temp2.getX() * cos(angle.getY()) + temp2.getZ() * sin(angle.getY()), temp2.getY(), -temp2.getX() * sin(angle.getY()) + temp2.getZ() * cos(angle.getY())); 
}

float Vector::dotProduct(Vector vector2) {
    return (x * vector2.getX()) + (y * vector2.getY()) + (z * vector2.getZ());
}

Vector Vector::crossProduct(Vector vector2) {
    return Vector(
        y * vector2.getZ() - z * vector2.getY(),
        z * vector2.getX() - x * vector2.getZ(),
        x * vector2.getY() - y * vector2.getX()
    );
}

Vector Vector::operator/(Vector divisor) {
    return Vector(fabs(divisor.getX()) > EPSILON ? x / divisor.getX() : 0,
                 fabs(divisor.getY()) > EPSILON ? y / divisor.getY() : 0,
                 fabs(divisor.getZ()) > EPSILON ? z / divisor.getZ() : 0);
}

Vector Vector::operator-(Vector vector2) {
    return Vector(x - vector2.getX(), y - vector2.getY(), z - vector2.getZ());
}

Vector Vector::operator+(Vector vector2) {
    return Vector(x + vector2.getX(), y + vector2.getY(), z + vector2.getZ());
}

Vector Vector::operator/(float divisor) {
    return Vector(x / divisor, y / divisor, z / divisor);
}

Vector Vector::operator*(float divisor) {
    return Vector(x * divisor, y * divisor, z * divisor);
}

Vector Vector::operator*(Vector other) {
    return Vector(x * other.getX(), y * other.getY(), z * other.getZ());
}

float Vector::operator[](int i) {
    if (i == 0) return x;
    else if (i == 1) return y;
    else if (i == 2) return z;
    throw std::runtime_error("Invalid Vector index");
}

bool Vector::equals(Vector other) {
    Vector diff = *this - other;
    return diff.getMagnitudeSqr() < S_EPSILON;
}

bool Vector::equals(Vector other, float tolerance) {
    Vector diff = *this - other;
    return diff.getMagnitudeSqr() < tolerance;
}

bool Vector::parallel(Vector other) {
    return this->equals(other) || this->equals(other * -1);
}

bool Vector::hasSameDirection(Vector other, float tolerance) {
    float coef;
    if (fabs(x) > tolerance) {
        coef = other.getX() / x;
    } else if (fabs(y) > tolerance) {
        coef = other.getY() / y;
    } else if (fabs(z) > tolerance) {
        coef = other.getZ() / z;
    } else {
        return other.isZero();
    }
    return this->equals(other / coef, tolerance);
}