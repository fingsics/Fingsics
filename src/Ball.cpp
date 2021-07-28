#include "../include/Ball.h"

using namespace std;

Ball::Ball(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, float mass, float elasticityCoef, Color color, float radius, int lats, int longs) :  Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->lats = lats;
    this->longs = longs;
    this->radius = radius;

    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia#List_of_3D_inertia_tensors
    float v = 2.0 / 5.0 * mass * radius * radius;
    this->baseInertiaTensor = Matrix(v, 0, 0, 0, v, 0, 0, 0, v);
    this->invertedInertiaTensor = baseInertiaTensor.inverse();
    this->obb = OBB(pos, Point(radius, radius, radius), rotationMatrix);
}

float Ball::getRadius() {
    return radius;
}

void Ball::drawObject() {
    Color darkColor = Color(255, 255, 255);

    glPushMatrix();

    glTranslatef(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixf(getOpenGLRotationMatrix());

    for(int i = 0; i <= lats; i++) {
        float lat0 = M_PI * (-0.5 + (float) (i - 1) / lats);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float) i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j <= longs; j++)
        {
            if (j > longs / 2) glColor3ub( color.getR(), color.getG(), color.getB());
            else glColor3ub( darkColor.getR(), darkColor.getG(), darkColor.getB());
            float lng = 2 * M_PI * (float) (j - 1) / longs;
            float x = cos(lng);
            float y = sin(lng);

            float s1, s2, t;
            s1 = ((float) i) / lats;
            s2 = ((float) i + 1) / lats;
            t = ((float) j) / longs;

            glNormal3d(x * zr0, y * zr0, z0);
            glVertex3d(radius * x * zr0, radius * y * zr0, radius * z0);

            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();

    }
    glPopMatrix();
 }

Matrix Ball::getInertiaTensorInverse() {
    return invertedInertiaTensor;
}

float Ball::getMinX() {
    return pos.getX() - radius;
}

float Ball::getMinY() {
    return pos.getY() - radius;
}

float Ball::getMinZ() {
    return pos.getZ() - radius;
}

float Ball::getMaxX() {
    return pos.getX() + radius;
}

float Ball::getMaxY() {
    return pos.getY() + radius;
}

float Ball::getMaxZ() {
    return pos.getZ() + radius;
}