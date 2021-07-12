#include "../include/Object.h"

using namespace std;

Object::Object(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point acceleration, double mass, double elasticityCoef, Color color) {
    this->isStatic = isStatic;
    this->pos = pos;
    this->mass = mass;
    this->elasticityCoef = elasticityCoef;
    this->vel = vel;
    this->angularVelocity = angularVelocity;
    this->acceleration = acceleration;
    this->color = color;
    this->id = id;
    this->rotationMatrix = Matrix(1,0,0,0,1,0,0,0,1) * Matrix(angle);
    this->numVelocityUpdatesThisFramePerAxis = Point();
    this->numAngularVelocityUpdatesThisFramePerAxis = Point();
}

bool Object::isMoving(){
    return vel.isZero();
}

double Object::getMass(){
    return mass;
}

string Object::getId() {
    return id;
}

Matrix Object::getRotationMatrix() {
    return rotationMatrix;
}

double* Object::getOpenGLRotationMatrix() {
    return rotationMatrix.getOpenGLRotationMatrix();
}

Point Object::getAngularVelocity() {
    return angularVelocity;
}

Point Object::getPos(){
    return pos;
}

Point Object::getVel(){
    return vel;
}

Point Object::getAcceleration() {
    return acceleration;
}

double Object::getElasticity() {
    return elasticityCoef;
}

bool Object::getIsStatic() {
    return isStatic;
}

void Object::setPos(Point pos) {
    this->pos = pos;
}

void Object::setVel(Point vel) {
    this->vel = vel;
}

void Object::updatePosAndVel(double secondsElapsed) {
    // Update position
    pos = pos + vel * secondsElapsed;

    // Update rotation matrix
    rotationMatrix = Matrix(angularVelocity * secondsElapsed) * rotationMatrix;

    // Update velocity
    vel = vel + acceleration * secondsElapsed;
}

Matrix Object::getInertiaTensor() {
    return rotationMatrix * baseInertiaTensor * rotationMatrix.transpose();
}

void Object::queueVelocityUpdates(Point velocity, Point angularVelocity) {
    velocityForUpdate = velocityForUpdate + velocity;
    angularVelocityForUpdate = angularVelocityForUpdate + angularVelocity;
    numVelocityUpdatesThisFramePerAxis = numVelocityUpdatesThisFramePerAxis.increaseIfNotZero(velocity);
    numAngularVelocityUpdatesThisFramePerAxis = numAngularVelocityUpdatesThisFramePerAxis.increaseIfNotZero(angularVelocity);
}

void Object::applyVelocityUpdates() {
    vel = vel + (velocityForUpdate / numVelocityUpdatesThisFramePerAxis);
    angularVelocity = angularVelocity + (angularVelocityForUpdate / numAngularVelocityUpdatesThisFramePerAxis);

    velocityForUpdate = Point();
    angularVelocityForUpdate = Point();
    numVelocityUpdatesThisFramePerAxis = Point();
    numAngularVelocityUpdatesThisFramePerAxis = Point();
}