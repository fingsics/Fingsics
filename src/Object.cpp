#include "../include/Object.h"

using namespace std;

Object::Object(string id, Point pos, Point vel, Point force, Point angle, Point angularVelocity, double mass, double elasticityCoef, Color color) {
    this->pos = pos;
    this->mass = mass;
    this->elasticityCoef = elasticityCoef;
    this->vel = vel;
    this->angle = angle;
    this->angularVelocity = angularVelocity;
    this->force = force;
    this->color = color;
    this->id = id;
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

Point Object::getAngle() {
    return angle;
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