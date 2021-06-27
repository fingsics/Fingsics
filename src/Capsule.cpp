#include "../include/Capsule.h"

using namespace std;

Capsule::Capsule(string id, Point pos, Point vel, Point force, double length, double radius, Point angle, Point angularVelocity, double mass, double elasticityCoef, Color color) : Object(id, pos, vel, force, angle, angularVelocity, mass, elasticityCoef, color) {
    this->length = length;
    this->radius = radius;
}

Point Capsule::getAxisDirection() {
    return Point(0,0,1).rotate(Point(angle));
}

double Capsule::getRadius() {
    return radius;
}

double Capsule::getLength() {
    return length;
}

Point Capsule::getCylinderEnd1() {
    return pos + getAxisDirection() * length / 2;
}

Point Capsule::getCylinderEnd2() {
    return pos - getAxisDirection() * length / 2;
}

void Capsule::draw() {
    glPushMatrix();
    glTranslatef(pos.getX(), pos.getY(), pos.getZ());
    // TODO: ROTATE PROPERLY
    glRotatef(angle.getX(),1,0,0);
    glRotatef(angle.getY(),0,1,0);
    glRotatef(angle.getZ(),0,0,1);
    glTranslatef(0, 0,-length / 2.0);
    glColor3ub(color.getR(), color.getG(), color.getB());
    double lats = LATS * 2;
    double longs = LONGS * 2;
    for (int i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= longs; j++)
        {
            double lng = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            double s1 = ((double)i) / lats;
            double s2 = ((double)i + 1) / lats;
            double t = ((double)j) / longs;

            glNormal3d(x * zr0, y * zr0, z0);
            glVertex3d(radius * x * zr0, radius * y * zr0, radius * z0);

            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();

        if (i == lats / 2) {
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= longs; j++)
            {
                double lng = 2 * M_PI * (double)(j - 1) / longs;
                double x = cos(lng);
                double y = sin(lng);

                double s1 = ((double)i) / lats;
                double s2 = ((double)i + 1) / lats;
                double t = ((double)j) / longs;

                glNormal3d(x * zr1, y * zr1, z1);
                glVertex3d(radius * x * zr1, radius * y * zr1, radius * z1);

                glNormal3d(x * zr1, y * zr1, z1);
                glVertex3d(radius * x * zr1, radius * y * zr1, radius * z1 + length);
            }
            glEnd();
            glTranslatef(0, 0, length);
        }
    }
    glPopMatrix();
}

void Capsule::updatePosAndVel(double secondsElapsed) {
    // Update velocity
    Point acceleration = force / mass;
    vel = vel + acceleration * secondsElapsed;

    // Update position
    pos = Point(pos.getX() + vel.getX() * secondsElapsed, pos.getY() + vel.getY() * secondsElapsed, pos.getZ() + vel.getZ() * secondsElapsed);

    // Update angular position
    double multiplier = secondsElapsed * 180 / M_PI;
    angle = Point(angle.getX() + angularVelocity.getX() * multiplier, angle.getY() + angularVelocity.getY() * multiplier, angle.getZ() + angularVelocity.getZ() * multiplier);
}

Matrix Capsule::getInertiaTensor() {
    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia#List_of_3D_inertia_tensors
    double x = 1.0 / 12.0 * (3 * radius * radius + length * length);
    double y = x;
    double z = 1.0 / 2.0 * mass * radius * radius;
    return Matrix(x, 0, 0,
                  0, y, 0,
                  0, 0, z);
}