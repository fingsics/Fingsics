#include "../include/Object.h"

using namespace std;

Object::Object(string id, Point pos, Point vel, double mass, Color color) {
    this->pos = pos;
    this->mass = mass;
    this->vel = vel;
    this->color = color;
    this->id = id;
}

void Object::setVel(Point vel){
    this->vel = vel;
}

bool Object::isMoving(){
    return vel.isZero();
}

void Object::decreaseVelocity(double time){
    double velDecrease = time / 3;
    float magnitude = vel.magnitude();

    if (velDecrease > 0){
        if (magnitude < velDecrease){
            vel = Point();
        }
        else{
            float decreaseFactor = magnitude / (magnitude - velDecrease);
            vel = vel / decreaseFactor;
        }
    }
}

void Object::updatePosAndVel(double time){
    double posFactor = time / 100;
    pos = pos + vel * posFactor;
    decreaseVelocity(time);
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

void Object::setPos(Point pos){
    this->pos = pos;
}