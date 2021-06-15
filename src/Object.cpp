#include "../include/Object.h"

using namespace std;

Object::Object(string id, double x, double y, double z, double mass, Color* color){
    this->posX = x;
    this->posY = y;
    this->posZ = z;
    this->mass = mass;
    this->velX = 0;
    this->velY = 0;
    this->velZ = 0;
    this->color = color;
    this->id = id;
}

void Object::setVelocity(Point* velVector){
    this->velX = velVector->getX();
    this->velY = velVector->getY();
    this->velZ = velVector->getZ();
}

bool Object::isMoving(){
    return (velX != 0 || velY != 0 || velZ != 0);
}

void Object::decreaseVelocity(double time){
    double velDecrease = time / 3;
    float magnitude = sqrt(pow(velX,2) + pow(velY,2) + pow(velZ,2));

    if (velDecrease > 0){
        if (magnitude < velDecrease){
            velX = 0;
            velY = 0;
            velZ = 0;
        }
        else{
            float decreaseFactor = magnitude / (magnitude - velDecrease);
            velX /= decreaseFactor;
            velY /= decreaseFactor;
            velZ /= decreaseFactor;
        }
    }
}

void Object::updatePosAndVel(double time){

    double posFactor = time / 100;

    // Update position
    double diffX = velX * posFactor;
    double diffY = velY * posFactor;
    double diffZ = velZ * posFactor;
    posX += velX * posFactor;
    posZ += velZ * posFactor;

    double distanceMoved = sqrt(pow(diffX,2) + pow(diffY,2) + pow(diffZ,2));

    decreaseVelocity(time);
}

double Object::getPosX(){
    return posX;
}

double Object::getPosY(){
    return posY;
}

double Object::getPosZ(){
    return posZ;
}

double Object::getMass(){
    return mass;
}

string Object::getId() {
    return id;
}

Point* Object::getPos(){
    return new Point(posX, posY, posZ);
}

Point* Object::getVel(){
    return new Point(velX, velY, velZ);
}

void Object::setPos(Point* pos){
    posX = pos->getX();
    posY = pos->getY();
    posZ = pos->getZ();
}