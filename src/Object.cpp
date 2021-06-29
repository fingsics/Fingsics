#include "../include/Object.h"

using namespace std;

Object::Object(string id, Point pos, Point vel, Point angle, Point angularVelocity, Point force, double mass, double elasticityCoef, Color color) {
    this->pos = pos;
    this->mass = mass;
    this->elasticityCoef = elasticityCoef;
    this->vel = vel;
    this->angularVelocity = angularVelocity;
    this->force = force;
    this->color = color;
    this->id = id;
    this->rotationMatrix = Matrix44();
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

Matrix44 Object::getRotationMatrix() {
    return rotationMatrix;
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

Point Object::getForce() {
    return force;
}

double Object::getElasticity() {
    return elasticityCoef;
}

void Object::setPos(Point pos){
    this->pos = pos;
}

void Object::setVel(Point vel) {
    this->vel = vel;
}

void Object::setAngularVelocity(Point angularVelocity) {
    this->angularVelocity = angularVelocity;
}

void Object::setForce(Point force) {
    this->force = force;
}

void Object::updatePosAndVel(double secondsElapsed) {
    // Update velocity
    Point acceleration = force / mass;
    vel = vel + acceleration * secondsElapsed;

    // Update position
    pos = Point(pos.getX() + vel.getX() * secondsElapsed, pos.getY() + vel.getY() * secondsElapsed, pos.getZ() + vel.getZ() * secondsElapsed);

    // Update rotation matrix
    double multiplier = secondsElapsed * 180 / M_PI;
    Matrix44 newRotation = Matrix44(angularVelocity * multiplier);
    rotationMatrix = rotationMatrix * newRotation;
}