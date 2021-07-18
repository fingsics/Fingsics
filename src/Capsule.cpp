#include "../include/Capsule.h"

using namespace std;

Capsule::Capsule(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, double mass, double elasticityCoef, Color color, double radius, double length, int lats, int longs) : Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->length = length;
    this->radius = radius;
    this->lats = lats;
    this->longs = longs;

    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia
    double extraLengthFactor = 2.0 / 3.0;
    double extraLength = (radius * 2.0) * extraLengthFactor;

    double cylinderLength = length + extraLength;
    double x = 1.0 / 12.0 * mass * (3.0 * radius * radius + cylinderLength * cylinderLength);
    double z = 1.0 / 2.0 * mass * radius * radius;
    this->baseInertiaTensor = Matrix(x, 0, 0, 0, x, 0, 0, 0, z);
    this->invertedInertiaTensor = (rotationMatrix * baseInertiaTensor * rotationMatrix.transpose()).inverse();
    this->obb = OBB(pos, Point(radius, radius, length / 2 + radius), rotationMatrix);
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

Point Capsule::getCylinderPositiveEnd() {
    return pos + getAxisDirection() * length / 2;
}

Point Capsule::getCylinderNegativeEnd() {
    return pos - getAxisDirection() * length / 2;
}

void Capsule::drawObject() {
    Color darkColor = Color(255, 255, 255);

    glPushMatrix();
    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixd(getOpenGLRotationMatrix());
    glTranslated(0, 0,-length / 2.0);

    double evenLats = (lats % 2 == 0) ? lats: lats + 1;
    double evenLongs = (longs % 2 == 0) ? longs : longs + 1;
    for (int i = 0; i <= evenLats; i++) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / evenLats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / evenLats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        glBegin(GL_QUAD_STRIP);

        for (int j = 0; j <= evenLongs; j++)
        {
            if (j > evenLongs / 2 == 0) glColor3ub(color.getR(), color.getG(), color.getB());
            else glColor3ub(darkColor.getR(), darkColor.getG(), darkColor.getB());
            double lng = 2 * M_PI * (double)(j - 1) / evenLongs;
            double x = cos(lng);
            double y = sin(lng);

            double s1 = ((double)i) / evenLats;
            double s2 = ((double)i + 1) / evenLats;
            double t = ((double)j) / evenLongs;

            glNormal3d(x * zr0, y * zr0, z0);
            glVertex3d(radius * x * zr0, radius * y * zr0, radius * z0);

            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(radius * x * zr1, radius * y * zr1, radius * z1);
        }
        glEnd();

        if (i == evenLats / 2) {
            glBegin(GL_QUAD_STRIP);
            for (int j = 0; j <= evenLongs; j++)
            {
                if (j > evenLongs / 2 == 0) glColor3ub(color.getR(), color.getG(), color.getB());
                else glColor3ub(darkColor.getR(), darkColor.getG(), darkColor.getB());
                double lng = 2 * M_PI * (double)(j - 1) / evenLongs;
                double x = cos(lng);
                double y = sin(lng);

                double s1 = ((double)i) / evenLats;
                double s2 = ((double)i + 1) / evenLats;
                double t = ((double)j) / evenLongs;

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

double* Capsule::getMins() {
    double* aabbMin = new double[3];
    aabbMin[0] = getMinX();
    aabbMin[1] = getMinY();
    aabbMin[2] = getMinZ();
    return aabbMin;
}

double* Capsule::getMaxes() {
    double* aabbMax = new double[3];
    aabbMax[0] = getMaxX();
    aabbMax[1] = getMaxY();
    aabbMax[2] = getMaxZ();
    return aabbMax;
}

double Capsule::getMinX() {
    double x1 = getCylinderPositiveEnd().getX();
    double x2 = getCylinderNegativeEnd().getX();
    return (x1 < x2 ? x1 : x2) - radius;
}

double Capsule::getMinY() {
    double y1 = getCylinderPositiveEnd().getY();
    double y2 = getCylinderNegativeEnd().getY();
    return (y1 < y2 ? y1 : y2) - radius;
}

double Capsule::getMinZ() {
    double z1 = getCylinderPositiveEnd().getZ();
    double z2 = getCylinderNegativeEnd().getZ();
    return (z1 < z2 ? z1 : z2) - radius;
}

double Capsule::getMaxX() {
    double x1 = getCylinderPositiveEnd().getX();
    double x2 = getCylinderNegativeEnd().getX();
    return (x1 > x2 ? x1 : x2) + radius;
}

double Capsule::getMaxY() {
    double y1 = getCylinderPositiveEnd().getY();
    double y2 = getCylinderNegativeEnd().getY();
    return (y1 > y2 ? y1 : y2) + radius;
}

double Capsule::getMaxZ() {
    double z1 = getCylinderPositiveEnd().getZ();
    double z2 = getCylinderNegativeEnd().getZ();
    return (z1 > z2 ? z1 : z2) + radius;
}