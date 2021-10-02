#include "../include/Capsule.h"

using namespace std;

Capsule::Capsule(string id, Color color, Point* positions, Matrix* rotationMatrices, int frames, float radius, float length, int lats, int longs) : Object(id, color, positions, rotationMatrices, frames) {
    this->length = length;
    this->radius = radius;
    this->lats = (lats % 2 == 0) ? lats : lats + 1;
    this->longs = (longs % 2 == 0) ? longs : longs + 1;

    this->baseInertiaTensor = Matrix();
    this->invertedInertiaTensor = Point();
    this->obb = frames > 0 ? OBB(positions[0], Point(radius, radius, length / 2 + radius), rotationMatrices[0]) : OBB();
    this->openGLArrayLength = ((this->lats + 1) * (this->longs + 1) + (this->longs + 1)) * 2 * 3;
    this->openGLVertices = new float[this->openGLArrayLength];
    this->openGLNormals = new float[this->openGLArrayLength];
}

Capsule::Capsule(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, float mass, float elasticityCoef, Color color, float radius, float length, int lats, int longs) : Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->length = length;
    this->radius = radius;
    this->lats = (lats % 2 == 0) ? lats : lats + 1;
    this->longs = (longs % 2 == 0) ? longs : longs + 1;
    this->axisDirection = rotationMatrix * Point(0, 0, 1);

    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia
    float extraLengthFactor = 2.0 / 3.0;
    float extraLength = (radius * 2.0) * extraLengthFactor;

    float cylinderLength = length + extraLength;
    float x = 1.0 / 12.0 * mass * (3.0 * radius * radius + cylinderLength * cylinderLength);
    float z = 1.0 / 2.0 * mass * radius * radius;
    this->baseInertiaTensor = Matrix(x, 0, 0, 0, x, 0, 0, 0, z);
    this->invertedInertiaTensor = (rotationMatrix * baseInertiaTensor * rotationMatrix.transpose()).inverse();
    this->obb = OBB(pos, Point(radius, radius, length / 2 + radius), rotationMatrix);
    this->openGLArrayLength = ((this->lats + 1) * (this->longs + 1) + (this->longs + 1)) * 2 * 3;
    this->openGLVertices = new float[this->openGLArrayLength];
    this->openGLNormals = new float[this->openGLArrayLength];
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

float Capsule::getLats() {
    return lats;
}

float Capsule::getLongs() {
    return longs;
}