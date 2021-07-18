#include "../include/Plane.h"

using namespace std;

Plane::Plane(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, double mass, double elasticityCoef, Color color, double drawLength, double drawWidth) :  Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->baseInertiaTensor = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->invertedInertiaTensor = Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0);
    this->drawLength = drawLength;
    this->drawWidth = drawWidth;
    this->obb = OBB(pos, Point(INF, EPSILON, INF), rotationMatrix);
}

Point Plane::getNormal(){
    return rotationMatrix * Point(0,1,0);
}

void Plane::drawObject() {
    glPushMatrix();
    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixd(getOpenGLRotationMatrix());
    glColor3ub(color.getR(), color.getG(), color.getB());
    glBegin(GL_QUADS);
    glVertex3d(-drawLength / 2, 0, drawWidth / 2);
    glVertex3d(drawLength / 2, 0, drawWidth / 2);
    glVertex3d(drawLength / 2, 0, -drawWidth / 2);
    glVertex3d(-drawLength / 2, 0, -drawWidth / 2);
    glEnd();
    glPopMatrix();
 }

Matrix Plane::getInertiaTensor() {
    return baseInertiaTensor;
}

void Plane::drawOBB() {
}


double* Plane::getMins() {
    throw "Plane AABBs aren't supported.";
}

double* Plane::getMaxes() {
    throw "Plane AABBs aren't supported.";
}

double Plane::getMinX() {
    throw "Plane AABBs aren't supported.";
}

double Plane::getMinY() {
    throw "Plane AABBs aren't supported.";
}

double Plane::getMinZ() {
    throw "Plane AABBs aren't supported.";
}

double Plane::getMaxX() {
    throw "Plane AABBs aren't supported.";
}

double Plane::getMaxY() {
    throw "Plane AABBs aren't supported.";
}

double Plane::getMaxZ() {
    throw "Plane AABBs aren't supported.";
}