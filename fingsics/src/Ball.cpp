#include "../include/Ball.h"

using namespace std;

Ball::Ball(string id, Color color, Vector* positions, Matrix* rotationMatrices, int frames, bool draw, float radius) : Object(id, color, positions, rotationMatrices, frames, draw) {
    this->radius = radius;
    this->baseInertiaTensor = Matrix();
    this->invertedInertiaTensor = Vector();
    this->obb = frames > 0 ? OBB(positions[0], Vector(radius, radius, radius), rotationMatrices[0]) : OBB();
}

Ball::Ball(string id, bool isStatic, Vector pos, Vector vel, Vector angle, Vector angularVelocity, Vector force, float mass, float elasticityCoef, Color color, bool draw, float radius) :  Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color, draw) {
    this->radius = radius;

    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia#List_of_3D_inertia_tensors
    float v = 2.0 / 5.0 * mass * radius * radius;
    this->baseInertiaTensor = Matrix(v, 0, 0, 0, v, 0, 0, 0, v);
    this->invertedInertiaTensor = baseInertiaTensor.inverse();
    this->obb = OBB(pos, Vector(radius, radius, radius), rotationMatrix);
}

float Ball::getRadius() {
    return radius;
}

Matrix Ball::getInertiaTensorInverse() {
    return invertedInertiaTensor;
}

float Ball::getMinX() {
    return position.getX() - radius;
}

float Ball::getMinY() {
    return position.getY() - radius;
}

float Ball::getMinZ() {
    return position.getZ() - radius;
}

float Ball::getMaxX() {
    return position.getX() + radius;
}

float Ball::getMaxY() {
    return position.getY() + radius;
}

float Ball::getMaxZ() {
    return position.getZ() + radius;
}
