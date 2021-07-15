#include "../include/OBB.h"

using namespace std;

OBB::OBB() {}

OBB::OBB(Point position, Point halfLengths, Matrix rotationMatrix) {
    this->position = position;
    this->halfLengths = halfLengths;
    this->normals = Matrix(Point(1,0,0), Point(0,1,0), Point(0,0,1));
    setRotation(rotationMatrix);
}

void OBB::setRotation(Matrix rotationMatrix) {
    Point rotatedNormal1 = rotationMatrix * normals.row(0);
    Point rotatedNormal2 = rotationMatrix * normals.row(1);
    Point rotatedNormal3 = rotationMatrix * normals.row(2);
    rotatedNormals = Matrix(rotatedNormal1, rotatedNormal2, rotatedNormal3);
}

void OBB::setPosition(Point position) {
    this->position = position;
}

Point OBB::getPosition() {
    return position;
}

Point OBB::getHalfLengths() {
    return halfLengths;
}

Matrix OBB::getRotatedNormals() {
    return rotatedNormals;
}