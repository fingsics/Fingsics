#include "../include/Object.h"

using namespace std;

Object::Object(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point acceleration, float mass, float elasticityCoef, Color color) {
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
    this->aabb = NULL;
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

float* Object::getOpenGLRotationMatrix() {
    return rotationMatrix.getOpenGLRotationMatrix();
}

Point Object::getAngularVelocity() {
    return angularVelocity;
}

Point Object::getPos() {
    return pos;
}

Point Object::getVel() {
    return vel;
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

void Object::setAABB(AABB* aabb) {
    this->aabb = aabb;
}


void Object::updatePosAndVel(float secondsElapsed) {
    if (!vel.isZero()) setPos(pos + vel * secondsElapsed);
    if (!angularVelocity.isZero()) setRotation(Matrix(angularVelocity * secondsElapsed) * rotationMatrix);
    if (!acceleration.isZero()) setVel(vel + acceleration * secondsElapsed);
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

    setVel(vel + velDiff);
    setAngularVelocity(angularVelocity + angVelDiff);
}

void Object::draw(bool drawOBB, bool drawAABB, bool drawHalfWhite) {
    this->drawObject(drawHalfWhite);
    if (drawOBB) this->drawOBB();
    if (drawAABB) this->drawAABB();
}

void Object::drawOBB() {
    Point dimensions = obb.getHalfLengths() * 2;
    Point pos = obb.getPosition();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3ub(220, 220, 0);
    glDisable(GL_LIGHTING);

    glPushMatrix();

    glTranslatef(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixf(getOpenGLRotationMatrix());
    glTranslatef(-dimensions.getX() / 2.0, -dimensions.getY() / 2.0, -dimensions.getZ() / 2.0);

    glBegin(GL_QUAD_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, dimensions.getZ());
    glVertex3f(0, dimensions.getY(), 0);
    glVertex3f(0, dimensions.getY(), dimensions.getZ());
    glVertex3f(dimensions.getX(), dimensions.getY(), 0);
    glVertex3f(dimensions.getX(), dimensions.getY(), dimensions.getZ());
    glVertex3f(dimensions.getX(), 0, 0);
    glVertex3f(dimensions.getX(), 0, dimensions.getZ());
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, dimensions.getZ());
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Object::drawAABB() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3ub(220, 0, 220);
    glDisable(GL_LIGHTING);

    glBegin(GL_QUAD_STRIP);
    glVertex3f(getMinX(), getMinY(), getMinZ());
    glVertex3f(getMaxX(), getMinY(), getMinZ());
    glVertex3f(getMinX(), getMaxY(), getMinZ());
    glVertex3f(getMaxX(), getMaxY(), getMinZ());
    glVertex3f(getMinX(), getMaxY(), getMaxZ());
    glVertex3f(getMaxX(), getMaxY(), getMaxZ());
    glVertex3f(getMinX(), getMinY(), getMaxZ());
    glVertex3f(getMaxX(), getMinY(), getMaxZ());
    glVertex3f(getMinX(), getMinY(), getMinZ());
    glVertex3f(getMaxX(), getMinY(), getMinZ());
    glEnd();

    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}