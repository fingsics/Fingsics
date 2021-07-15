#include "../include/Object.h"

using namespace std;

Impulse::Impulse(Point normal, Point tangent, double magnitude, double mass) {
    this->normal = normal;
    this->tangent = tangent;
    this->magnitude = magnitude;
    this->mass = mass;
}

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
    this->queuedImpulses = list<Impulse>();
    this->velCollisionMassPerAxis = Point();
    this->angVelCollisionMassPerAxis = Point();
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

OBB Object::getOBB() {
    return obb;
}

void Object::setPos(Point pos) {
    this->pos = pos;
    obb.setPosition(pos);
}

void Object::setVel(Point vel) {
    this->vel = vel;
}

void Object::setAngularVelocity(Point angularVelocity) {
    this->angularVelocity = angularVelocity;
}

void Object::setRotation(Matrix rotationMatrix) {
    this->rotationMatrix = rotationMatrix;
    invertedInertiaTensor = (rotationMatrix * baseInertiaTensor * rotationMatrix.transpose()).inverse();
    obb.setRotation(rotationMatrix);
}

void Object::updatePosAndVel(double secondsElapsed) {
    if (!vel.isZero()) setPos(pos + vel * secondsElapsed);
    if (!angularVelocity.isZero()) setRotation(Matrix(angularVelocity * secondsElapsed) * rotationMatrix);
    if (!acceleration.isZero()) setVel(vel + acceleration * secondsElapsed);
}

Matrix Object::getInertiaTensorInverse() {
    return invertedInertiaTensor;
}

void Object::queueImpulse(Point normal, Point tangent, double magnitude, double mass) {
    queuedImpulses.insert(queuedImpulses.begin(), Impulse(normal, tangent, magnitude, mass));
    velCollisionMassPerAxis = velCollisionMassPerAxis.addIfComponentNotZero(normal, mass);
    angVelCollisionMassPerAxis = angVelCollisionMassPerAxis.addIfComponentNotZero(tangent, mass);
}

void Object::applyQueuedImpulses() {
    for (auto it = queuedImpulses.begin(); it != queuedImpulses.end(); ++it) {
        Point velProportion = Point(it->mass, it->mass, it->mass) / velCollisionMassPerAxis;
        Point angVelProportion = Point(it->mass, it->mass, it->mass) / angVelCollisionMassPerAxis;
        applyImpulse(it->normal * velProportion * it->magnitude, it->tangent * angVelProportion * it->magnitude);
    }

    queuedImpulses.clear();
    velCollisionMassPerAxis = Point();
    angVelCollisionMassPerAxis = Point();
}

void Object::applyImpulse(Point normal, Point tangent) {
    Point velDiff = normal / mass;
    Point angVelDiff = getInertiaTensorInverse() * tangent;

    vel = vel + velDiff;
    angularVelocity = angularVelocity + angVelDiff;
}