#include "../include/Capsule.h"

using namespace std;

Capsule::Capsule(string id, Color color, Point* positions, Matrix* rotationMatrices, int frames, bool draw, bool rocket, float radius, float length) : Object(id, color, positions, rotationMatrices, frames, draw) {
    this->length = length;
    this->radius = radius;

    this->drawRocket = rocket;
    this->baseInertiaTensor = Matrix();
    this->invertedInertiaTensor = Point();
    this->obb = frames > 0 ? OBB(positions[0], Point(radius, radius, length / 2 + radius), rotationMatrices[0]) : OBB();
}

Capsule::Capsule(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, float mass, float elasticityCoef, Color color, bool draw, bool rocket, float radius, float length) : Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color, draw) {
    this->length = length;
    this->radius = radius;
    this->axisDirection = rotationMatrix * Point(0, 0, 1);
    this->drawRocket = rocket;

    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia
    float extraLengthFactor = 2.0 / 3.0;
    float extraLength = (radius * 2.0) * extraLengthFactor;

    float cylinderLength = length + extraLength;
    float x = 1.0 / 12.0 * mass * (3.0 * radius * radius + cylinderLength * cylinderLength);
    float z = 1.0 / 2.0 * mass * radius * radius;
    this->baseInertiaTensor = Matrix(x, 0, 0, 0, x, 0, 0, 0, z);
    this->invertedInertiaTensor = (rotationMatrix * baseInertiaTensor * rotationMatrix.transpose()).inverse();
    this->obb = OBB(pos, Point(radius, radius, length / 2 + radius), rotationMatrix);
}

void Capsule::setRotation(Matrix rotationMatrix) {
    this->rotationMatrix = rotationMatrix;
    invertedInertiaTensor = (rotationMatrix * baseInertiaTensor * rotationMatrix.transpose()).inverse();
    obb.setRotation(rotationMatrix);
    axisDirection = rotationMatrix * Point(0, 0, 1);
}

Point Capsule::getAxisDirection() {
    return axisDirection;
}

float Capsule::getRadius() {
    return radius;
}

float Capsule::getLength() {
    return length;
}

Point Capsule::getCylinderPositiveEnd() {
    return position + axisDirection * length / 2;
}

Point Capsule::getCylinderNegativeEnd() {
    return position - axisDirection * length / 2;
}

float Capsule::getMinX() {
    float x1 = getCylinderPositiveEnd().getX();
    float x2 = getCylinderNegativeEnd().getX();
    return (x1 < x2 ? x1 : x2) - radius;
}

float Capsule::getMinY() {
    float y1 = getCylinderPositiveEnd().getY();
    float y2 = getCylinderNegativeEnd().getY();
    return (y1 < y2 ? y1 : y2) - radius;
}

float Capsule::getMinZ() {
    float z1 = getCylinderPositiveEnd().getZ();
    float z2 = getCylinderNegativeEnd().getZ();
    return (z1 < z2 ? z1 : z2) - radius;
}

float Capsule::getMaxX() {
    float x1 = getCylinderPositiveEnd().getX();
    float x2 = getCylinderNegativeEnd().getX();
    return (x1 > x2 ? x1 : x2) + radius;
}

float Capsule::getMaxY() {
    float y1 = getCylinderPositiveEnd().getY();
    float y2 = getCylinderNegativeEnd().getY();
    return (y1 > y2 ? y1 : y2) + radius;
}

float Capsule::getMaxZ() {
    float z1 = getCylinderPositiveEnd().getZ();
    float z2 = getCylinderNegativeEnd().getZ();
    return (z1 > z2 ? z1 : z2) + radius;
}

bool Capsule::getDrawRocket() {
    return drawRocket;
}