#include "../include/Plane.h"

using namespace std;

Plane::Plane(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, float mass, float elasticityCoef, Color color, float drawLength, float drawWidth) :  Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->baseInertiaTensor = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->invertedInertiaTensor = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->drawLength = drawLength / 2.0;
    this->drawWidth = drawWidth / 2.0;
    this->obb = OBB(pos, Point(INF, EPSILON, INF), rotationMatrix);
}

Point Plane::getNormal(){
    return rotationMatrix * Point(0,1,0);
}

void Plane::drawObject() {
    glPushMatrix();
    glTranslatef(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixf(getOpenGLRotationMatrix());
    glColor3ub(color.getR(), color.getG(), color.getB());
    glBegin(GL_QUADS);
    glVertex3d(-drawLength, 0, drawWidth);
    glVertex3d(drawLength, 0, drawWidth);
    glVertex3d(drawLength, 0, -drawWidth);
    glVertex3d(-drawLength, 0, -drawWidth);
    glEnd();
    glPopMatrix();
 }

Matrix Plane::getInertiaTensor() {
    return baseInertiaTensor;
}

void Plane::drawOBB() {
}

void Plane::drawAABB() {
}

float Plane::getMinX() {
    throw "Plane AABBs aren't supported.";
}

float Plane::getMinY() {
    throw "Plane AABBs aren't supported.";
}

float Plane::getMinZ() {
    throw "Plane AABBs aren't supported.";
}

float Plane::getMaxX() {
    throw "Plane AABBs aren't supported.";
}

float Plane::getMaxY() {
    throw "Plane AABBs aren't supported.";
}

float Plane::getMaxZ() {
    throw "Plane AABBs aren't supported.";
}