#include "../include/OBB.h"

using namespace std;

OBB::OBB() {}

OBB::OBB(Point position, Point halfLengths, Matrix rotationMatrix) {
    this->position = position;
    this->halfLengths = halfLengths;
    setRotation(rotationMatrix);
}

void OBB::setRotation(Matrix rotationMatrix) {
    Point normal1 = rotationMatrix * Point(1, 0, 0);
    Point normal2 = rotationMatrix * Point(0, 1, 0);
    Point normal3 = rotationMatrix * Point(0, 0, 1);
    normals = Matrix(normal1, normal2, normal3);
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

Matrix OBB::getNormals() {
    return normals;
}