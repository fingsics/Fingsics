#include "../include/Capsule.h"

using namespace std;

Capsule::Capsule(string id, Point pos, Point vel, Point force, double length, double radius, Point angle, Point angularVelocity, double mass, double elasticityCoef, Color color) : Object(id, pos, vel, force, mass, elasticityCoef, color) {
    this->length = length;
    this->radius = radius;
    this->angle = angle;
    this->angularVelocity = angularVelocity;
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

Point Capsule::getAngle() {
    return angle;
}

Point Capsule::getAngularVelocity() {
    return angularVelocity;
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

void Capsule::updatePosAndVel(double secondsElapsed, Room room) {
    // Check collision with floor

    // Check collision with walls

    // Update velocity
    Point acceleration = force / mass;
    vel = vel + acceleration * secondsElapsed;

    // Update position
    double epsilon = 0.001;
    pos = Point(pos.getX() + vel.getX() * secondsElapsed, max(radius - epsilon, pos.getY() + vel.getY() * secondsElapsed), pos.getZ() + vel.getZ() * secondsElapsed);

    // Update angular velocity

    // Update angle
}