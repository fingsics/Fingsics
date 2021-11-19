#include "../include/OBB.h"

using namespace std;

OBB::OBB() {}

OBB::OBB(Vector position, Vector halfLengths, Matrix rotationMatrix) {
    this->position = position;
    this->halfLengths = halfLengths;
    setRotation(rotationMatrix);
}

void OBB::setRotation(Matrix rotationMatrix) {
    Vector normal1 = rotationMatrix * Vector(1, 0, 0);
    Vector normal2 = rotationMatrix * Vector(0, 1, 0);
    Vector normal3 = rotationMatrix * Vector(0, 0, 1);
    normals = Matrix(normal1, normal2, normal3);
}

void OBB::setPosition(Vector position) {
    this->position = position;
}

Vector OBB::getPosition() {
    return position;
}

Vector OBB::getHalfLengths() {
    return halfLengths;
}

Matrix OBB::getNormals() {
    return normals;
}