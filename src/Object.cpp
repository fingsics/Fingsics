#include "../include/Object.h"

using namespace std;

// Draw only object
Object::Object(string id, Color color, Point* positions, Matrix* rotationMatrices, int frames, bool draw) {
    this->id = id;
    this->replayMode = true;
    this->positions = positions;
    this->rotationMatrices = rotationMatrices;
    this->frames = frames;
    this->color = color;

    this->isStatic = false;
    this->position = Point();
    this->mass = 0;
    this->elasticityCoef = 0;
    this->velocity = Point();
    this->angularVelocity = Point();
    this->acceleration = Point();
    this->rotationMatrix = Matrix();
    this->queuedImpulses = list<Impulse>();
    this->velCollisionMassPerAxis = Point();
    this->angVelCollisionMassPerAxis = Point();
    this->aabb = NULL;
    this->draw = draw;
}

Object::Object(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point acceleration, float mass, float elasticityCoef, Color color, bool draw) {
    this->isStatic = isStatic;
    this->position = pos;
    this->mass = mass;
    this->elasticityCoef = elasticityCoef;
    this->velocity = vel;
    this->angularVelocity = angularVelocity;
    this->acceleration = acceleration;
    this->color = color;
    this->id = id;
    this->rotationMatrix = Matrix(1,0,0,0,1,0,0,0,1) * Matrix(angle);
    this->queuedImpulses = list<Impulse>();
    this->velCollisionMassPerAxis = Point();
    this->angVelCollisionMassPerAxis = Point();
    this->aabb = NULL;
    this->draw = draw;

    this->replayMode = false;
    this->positions = NULL;
    this->rotationMatrices = NULL;
    this->frames = -1;
}

float Object::getMass() {
    return mass;
}

string Object::getId() {
    return id;
}

Matrix Object::getRotationMatrix() {
    return rotationMatrix;
}

Point Object::getAngularVelocity() {
    return angularVelocity;
}

Point Object::getPosition() {
    return position;
}

Point Object::getVelocity() {
    return velocity;
}

Point Object::getAcceleration() {
    return acceleration;
}

float Object::getElasticity() {
    return elasticityCoef;
}

bool Object::getIsStatic() {
    return isStatic;
}

OBB Object::getOBB() {
    return obb;
}

AABB* Object::getAABB() {
    return aabb;
}

Color Object::getColor() {
    return color;
}

void Object::setPosition(Point pos) {
    this->position = pos;
    obb.setPosition(pos);
}

void Object::setVelocity(Point vel) {
    this->velocity = vel;
}

void Object::setAngularVelocity(Point angularVelocity) {
    this->angularVelocity = angularVelocity;
}

void Object::setRotation(Matrix rotationMatrix) {
    this->rotationMatrix = rotationMatrix;
    invertedInertiaTensor = (rotationMatrix * baseInertiaTensor * rotationMatrix.transpose()).inverse();
    obb.setRotation(rotationMatrix);
}

void Object::setAABB(AABB* aabb) {
    this->aabb = aabb;
}

void Object::goToFrame(int frame) {
    setPosition(positions[frame]);
    setRotation(rotationMatrices[frame]);
}

void Object::updatePositionAndVelocity(float secondsElapsed) {
    if (!velocity.isZero()) setPosition(position + velocity * secondsElapsed);
    if (!angularVelocity.isZero()) setRotation(Matrix(angularVelocity * secondsElapsed) * rotationMatrix);
    if (!acceleration.isZero()) setVelocity(velocity + acceleration * secondsElapsed);
}

Matrix Object::getInertiaTensorInverse() {
    return invertedInertiaTensor;
}

void Object::queueImpulse(Point normal, Point tangent, float magnitude, float mass) {
    normal = normal.roundToZeroIfNear();
    tangent = tangent.roundToZeroIfNear();
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

    setVelocity(velocity + velDiff);
    setAngularVelocity(angularVelocity + angVelDiff);
}

bool Object::getDraw() {
    return draw;
}