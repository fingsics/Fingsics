#include "../include/Capsule.h"

using namespace std;

Capsule::Capsule(string id, Point pos, Point vel, Point angle, Point angularVelocity, Point force, double mass, double elasticityCoef, Color color, double radius, double length) : Object(id, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->length = length;
    this->radius = radius;

    //double x = 1.0 / 12.0 * (3 * radius * radius + length * length);
    //double z = 1.0 / 2.0 * mass * radius * radius;

    // https://www.gamedev.net/tutorials/programming/math-and-physics/capsule-inertia-tensor-r3856/
    double mcy = length * radius * radius * M_PI;
    double mhs = 2 * radius * radius * radius * M_PI / 3;
    double m = mcy + 2 * mhs;
    double x = mcy * (length * length / 12 + radius * radius / 4) + 2 * mhs * (2 * radius * radius / 5 + length * length / 2 + 3 * length * radius / 8);
    double z = mcy * radius * radius / 2 + 2 * mhs * 2 * radius * radius / 5;

    double density = mass / (M_PI * radius * radius * length + (4/3) * M_PI * radius * radius * radius);

    this->baseInertiaTensor = Matrix(x, 0, 0, 0, x, 0, 0, 0, z) * density;
}

Point Capsule::getAxisDirection() {
    return rotationMatrix * Point(0,0,1);
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
    Color darkColor = Color(255, 255, 255);

    glPushMatrix();
    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixd(getOpenGLRotationMatrix());
    glTranslated(0, 0,-length / 2.0);

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
        glBegin(GL_QUAD_STRIP);

        for (int j = 0; j <= longs; j++)
        {
            if (j > longs / 2 == 0) glColor3ub(color.getR(), color.getG(), color.getB());
            else glColor3ub(darkColor.getR(), darkColor.getG(), darkColor.getB());
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
                if (j > longs / 2 == 0) glColor3ub(color.getR(), color.getG(), color.getB());
                else glColor3ub(darkColor.getR(), darkColor.getG(), darkColor.getB());
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