#include "../include/Ball.h"

using namespace std;

Ball::Ball(string id, Color color, Point* positions, Matrix* rotationMatrices, int frames, float radius, int lats, int longs) : Object(id, color, positions, rotationMatrices, frames) {
    this->radius = radius;
    this->lats = (lats % 2 == 0) ? lats : lats + 1;
    this->longs = (longs % 2 == 0) ? longs : longs + 1;
    this->baseInertiaTensor = Matrix();
    this->invertedInertiaTensor = Point();
    this->obb = frames > 0 ? OBB(positions[0], Point(radius, radius, radius), rotationMatrices[0]) : OBB();
    this->openGLArrayLength = ((this->lats + 1) * (this->longs + 1)) * 2 * 3;
    this->openGLVertices = new float[this->openGLArrayLength];
    this->openGLNormals = new float[this->openGLArrayLength];
}

Ball::Ball(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, float mass, float elasticityCoef, Color color, float radius, int lats, int longs) :  Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->lats = (lats % 2 == 0) ? lats : lats + 1;
    this->longs = (longs % 2 == 0) ? longs : longs + 1;
    this->radius = radius;

    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia#List_of_3D_inertia_tensors
    float v = 2.0 / 5.0 * mass * radius * radius;
    this->baseInertiaTensor = Matrix(v, 0, 0, 0, v, 0, 0, 0, v);
    this->invertedInertiaTensor = baseInertiaTensor.inverse();
    this->obb = OBB(pos, Point(radius, radius, radius), rotationMatrix);
    this->openGLArrayLength = ((this->lats + 1) * (this->longs + 1)) * 2 * 3;
    this->openGLVertices = new float[this->openGLArrayLength];
    this->openGLNormals = new float[this->openGLArrayLength];
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

int Ball::getLats() {
    return lats;
}

int Ball::getLongs() {
    return longs;
}