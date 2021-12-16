#include "../include/Tile.h"

using namespace std;

Tile::Tile(string id, Color color, Vector* positions, Matrix* rotationMatrices, int frames, bool draw, float length, float width) : Object(id, color, positions, rotationMatrices, frames, draw) {
    this->axis1Length = length;
    this->axis2Length = width;
    if (frames > 0) {
        this->position = positions[0];
        this->setRotation(rotationMatrices[0]);
        this->obb = OBB(positions[0], Vector(length, EPSILON, width), rotationMatrices[0]);
    }
}

Tile::Tile(string id, bool isStatic, Vector pos, Vector vel, Vector angle, Vector angularVelocity, Vector force, float mass, float elasticityCoef, Color color, bool draw, float length, float width) :  Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color, draw) {
    this->baseInertiaTensor = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->invertedInertiaTensor = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->axis1Length = length;
    this->axis2Length = width;
    this->obb = OBB(pos, Vector(length, EPSILON, width), rotationMatrix);
    this->setRotation(rotationMatrix);
}

Vector Tile::getAxis1() {
    return axis1;
}

Vector Tile::getAxis2() {
    return axis2;
}

Vector Tile::getEnd1() {
    return end1;
}

Vector Tile::getEnd2() {
    return end2;
}

Vector Tile::getEnd3() {
    return end3;
}

Vector Tile::getEnd4() {
    return end4;
}

float Tile::getAxis1Length() {
    return axis1Length;
}

float Tile::getAxis2Length() {
    return axis2Length;
}

Vector Tile::getNormal() {
    return rotationMatrix * Vector(0, 1, 0);
}

void Tile::setRotation(Matrix rotationMatrix) {
    this->rotationMatrix = rotationMatrix;
    obb.setRotation(rotationMatrix);
    axis1 = rotationMatrix * Vector(1, 0, 0);
    axis2 = rotationMatrix * Vector(0, 0, 1);
    end1 = position + axis1 * axis1Length / 2 + axis2 * axis2Length / 2;
    end2 = position - axis1 * axis1Length / 2 + axis2 * axis2Length / 2;
    end3 = position + axis1 * axis1Length / 2 - axis2 * axis2Length / 2;
    end4 = position - axis1 * axis1Length / 2 - axis2 * axis2Length / 2;
}

Matrix Tile::getInertiaTensor() {
    return baseInertiaTensor;
}

float Tile::getMinX() {
    return min(min(min(end1.getX(), end2.getX()), end3.getX()), end4.getX());
}

float Tile::getMinY() {
    return min(min(min(end1.getY(), end2.getY()), end3.getY()), end4.getY());
}

float Tile::getMinZ() {
    return min(min(min(end1.getZ(), end2.getZ()), end3.getZ()), end4.getZ());
}

float Tile::getMaxX() {
    return max(max(max(end1.getX(), end2.getX()), end3.getX()), end4.getX());
}

float Tile::getMaxY() {
    return max(max(max(end1.getY(), end2.getY()), end3.getY()), end4.getY());
}

float Tile::getMaxZ() {
    return max(max(max(end1.getZ(), end2.getZ()), end3.getZ()), end4.getZ());
}