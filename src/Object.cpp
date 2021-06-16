#include "../include/Object.h"

using namespace std;

Object::Object(string id, Point pos, Point vel, Point force, double mass, Color color) {
    this->pos = pos;
    this->mass = mass;
    this->vel = vel;
    this->force = force;
    this->color = color;
    this->id = id;
}

bool Object::isMoving(){
    return vel.isZero();
}

void Object::updatePosAndVel(double secondsElapsed){
    // Update velocity
    Point acceleration = force / mass;
    vel = vel + acceleration * secondsElapsed;

    // Update position
    pos = pos + vel * secondsElapsed;
}

double Object::getMass(){
    return mass;
}

string Object::getId() {
    return id;
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

void Object::setPos(Point pos){
    this->pos = pos;
}

void Object::setVel(Point vel) {
    this->vel = vel;
}

void Object::setForce(Point force) {
    this->force = force;
}