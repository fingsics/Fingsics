#include "../include/Capsule.h"

using namespace std;

Capsule::Capsule(string id, Color color, Point* positions, Matrix* rotationMatrices, int frames, float radius, float length, int lats, int longs) : Object(id, color, positions, rotationMatrices, frames) {
    this->length = length;
    this->radius = radius;
    this->lats = lats;
    this->longs = longs;

    this->baseInertiaTensor = Matrix();
    this->invertedInertiaTensor = Point();
    this->obb = OBB();
}

Capsule::Capsule(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, float mass, float elasticityCoef, Color color, float radius, float length, int lats, int longs) : Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->length = length;
    this->radius = radius;
    this->lats = lats;
    this->longs = longs;
    this->axisDirection = rotationMatrix * Point(0, 0, 1);

    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia
    float extraLengthFactor = 2.0 / 3.0;
    float extraLength = (radius * 2.0) * extraLengthFactor;

    float cylinderLength = length + extraLength;
    float x = 1.0 / 12.0 * mass * (3.0 * radius * radius + cylinderLength * cylinderLength);
    float z = 1.0 / 2.0 * mass * radius * radius;
    this->baseInertiaTensor = Matrix(x, 0, 0, 0, x, 0, 0, 0, z);
    this->invertedInertiaTensor = (rotationMatrix * baseInertiaTensor * rotationMatrix.transpose()).inverse();
    this->obb = OBB(pos, Point(radius, radius, length / 2 + radius), rotationMatrix);
}

void Capsule::setRotation(Matrix rotationMatrix) {
    this->rotationMatrix = rotationMatrix;
    invertedInertiaTensor = (rotationMatrix * baseInertiaTensor * rotationMatrix.transpose()).inverse();
    obb.setRotation(rotationMatrix);
    axisDirection = rotationMatrix * Point(0, 0, 1);
}

Point Capsule::getAxisDirection() {
    return axisDirection;
}

float Capsule::getRadius() {
    return radius;
}

float Capsule::getLength() {
    return length;
}

Point Capsule::getCylinderPositiveEnd() {
    return position + axisDirection * length / 2;
}

Point Capsule::getCylinderNegativeEnd() {
    return position - axisDirection * length / 2;
}

void Capsule::drawObject(bool drawHalfWhite, int frame) {
    Color white = Color(255, 255, 255);
    glColor3ub(color.getR(), color.getG(), color.getB());

    Point pos = replayMode ? positions[frame] : position;
    float* mat = (replayMode ? rotationMatrices[frame] : rotationMatrix).getOpenGLRotationMatrix();

    glPushMatrix();
    glTranslatef(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixf(mat);
    glTranslatef(0, 0,-length / 2.0);

    float evenLats = (lats % 2 == 0) ? lats : lats + 1;
    float evenLongs = (longs % 2 == 0) ? longs : longs + 1;
    for (int i = 0; i <= evenLats; i++) {
        float lat0 = M_PI * (-0.5 + (float)(i - 1) / evenLats);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float)i / evenLats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        glBegin(GL_QUAD_STRIP);

        for (int j = 0; j <= evenLongs; j++)
        {
            if (drawHalfWhite) {
                if (j > evenLongs / 2 == 0) glColor3ub(color.getR(), color.getG(), color.getB());
                else glColor3ub(white.getR(), white.getG(), white.getB());
            }
            
            float lng = 2 * M_PI * (float)(j - 1) / evenLongs;
            float x = cos(lng);
            float y = sin(lng);

            float s1 = ((float)i) / evenLats;
            float s2 = ((float)i + 1) / evenLats;
            float t = ((float)j) / evenLongs;

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
                if (drawHalfWhite) {
                    if (j > evenLongs / 2 == 0) glColor3ub(color.getR(), color.getG(), color.getB());
                    else glColor3ub(white.getR(), white.getG(), white.getB());
                }

                float lng = 2 * M_PI * (float)(j - 1) / evenLongs;
                float x = cos(lng);
                float y = sin(lng);

                float s1 = ((float)i) / evenLats;
                float s2 = ((float)i + 1) / evenLats;
                float t = ((float)j) / evenLongs;

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

float Capsule::getMinX() {
    float x1 = getCylinderPositiveEnd().getX();
    float x2 = getCylinderNegativeEnd().getX();
    return (x1 < x2 ? x1 : x2) - radius;
}

float Capsule::getMinY() {
    float y1 = getCylinderPositiveEnd().getY();
    float y2 = getCylinderNegativeEnd().getY();
    return (y1 < y2 ? y1 : y2) - radius;
}

float Capsule::getMinZ() {
    float z1 = getCylinderPositiveEnd().getZ();
    float z2 = getCylinderNegativeEnd().getZ();
    return (z1 < z2 ? z1 : z2) - radius;
}

float Capsule::getMaxX() {
    float x1 = getCylinderPositiveEnd().getX();
    float x2 = getCylinderNegativeEnd().getX();
    return (x1 > x2 ? x1 : x2) + radius;
}

float Capsule::getMaxY() {
    float y1 = getCylinderPositiveEnd().getY();
    float y2 = getCylinderNegativeEnd().getY();
    return (y1 > y2 ? y1 : y2) + radius;
}

float Capsule::getMaxZ() {
    float z1 = getCylinderPositiveEnd().getZ();
    float z2 = getCylinderNegativeEnd().getZ();
    return (z1 > z2 ? z1 : z2) + radius;
}