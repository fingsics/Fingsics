#include "../include/Tile.h"

using namespace std;

Tile::Tile(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, float mass, float elasticityCoef, Color color, float length, float width) :  Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->baseInertiaTensor = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->invertedInertiaTensor = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->axis1Length = length;
    this->axis2Length = width;
    this->obb = OBB(pos, Point(width, EPSILON, length), rotationMatrix);
    this->axis1 = rotationMatrix * Point(1, 0, 0);
    this->axis2 = rotationMatrix * Point(0, 0, 1);
    this->updateEnds();
}

Point Tile::getAxis1() {
    return axis1;
}

Point Tile::getAxis2() {
    return axis2;
}

Point Tile::getEnd1() {
    return end1;
}

Point Tile::getEnd2() {
    return end2;
}

Point Tile::getEnd3() {
    return end3;
}

Point Tile::getEnd4() {
    return end4;
}

float Tile::getAxis1Length() {
    return axis1Length;
}

float Tile::getAxis2Length() {
    return axis2Length;
}

Point Tile::getNormal() {
    return rotationMatrix * Point(0, 1, 0);
}

void Tile::updateEnds() {
    end1 = pos + axis1 * axis1Length / 2 + axis2 * axis2Length / 2;
    end2 = pos - axis1 * axis1Length / 2 + axis2 * axis2Length / 2;
    end3 = pos + axis1 * axis1Length / 2 - axis2 * axis2Length / 2;
    end4 = pos - axis1 * axis1Length / 2 - axis2 * axis2Length / 2;
}

void Tile::setRotation(Matrix rotationMatrix) {
    this->rotationMatrix = rotationMatrix;
    obb.setRotation(rotationMatrix);
    axis1 = rotationMatrix * Point(1, 0, 0);
    axis2 = rotationMatrix * Point(0, 0, 1);
    updateEnds();
}

void Tile::drawObject(bool) {
    glPushMatrix();
    glTranslatef(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixf(getOpenGLRotationMatrix());
    glColor3ub(color.getR(), color.getG(), color.getB());
    glBegin(GL_QUADS);
    glVertex3d(-axis1Length / 2.0, 0, axis2Length / 2.0);
    glVertex3d(axis1Length / 2.0, 0, axis2Length / 2.0);
    glVertex3d(axis1Length / 2.0, 0, -axis2Length / 2.0);
    glVertex3d(-axis1Length / 2.0, 0, -axis2Length / 2.0);
    glEnd();
    glPopMatrix();
 }

Matrix Tile::getInertiaTensor() {
    return baseInertiaTensor;
}

void Tile::drawOBB() {
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