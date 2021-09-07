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

void Ball::drawObject() {
    glPushMatrix();

    glTranslatef(position.getX(), position.getY(), position.getZ());
    glMultMatrixf(rotationMatrix.getOpenGLRotationMatrix());

    int arrayIndex = 0;

    for (int i = 0; i <= lats; i++) {
        float lat0 = M_PI * (-0.5 + (float)(i - 1) / lats);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float)i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        for (int j = 0; j <= longs; j++) {
            float lng = 2 * M_PI * (float)(j - 1) / longs;
            float x = cos(lng);
            float y = sin(lng);

            float s1, s2, t;
            s1 = ((float)i) / lats;
            s2 = ((float)i + 1) / lats;
            t = ((float)j) / longs;

            openGLNormals[arrayIndex] = x * zr0;
            openGLNormals[arrayIndex + 1] = y * zr0;
            openGLNormals[arrayIndex + 2] = z0;

            openGLVertices[arrayIndex] = radius * x * zr0;
            openGLVertices[arrayIndex + 1] = radius * y * zr0;
            openGLVertices[arrayIndex + 2] = radius * z0;

            openGLNormals[arrayIndex + 3] = x * zr1;
            openGLNormals[arrayIndex + 4] = y * zr1;
            openGLNormals[arrayIndex + 5] = z1;

            openGLVertices[arrayIndex + 3] = radius * x * zr1;
            openGLVertices[arrayIndex + 4] = radius * y * zr1;
            openGLVertices[arrayIndex + 5] = radius * z1;

            arrayIndex += 6;
        }
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, openGLVertices);
    glNormalPointer(GL_FLOAT, 0, openGLNormals);
    glColor3ub(color.getR(), color.getG(), color.getB());
    glDrawArrays(GL_QUAD_STRIP, 0, openGLArrayLength / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopMatrix();
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