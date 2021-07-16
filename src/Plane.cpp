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