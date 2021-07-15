#include "../include/Ball.h"

using namespace std;

Ball::Ball(string id, bool isStatic, Point pos, Point vel, Point angle, Point angularVelocity, Point force, double mass, double elasticityCoef, Color color, double radius, int lats, int longs) :  Object(id, isStatic, pos, vel, angle, angularVelocity, force, mass, elasticityCoef, color) {
    this->lats = lats;
    this->longs = longs;
    this->radius = radius;

    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia#List_of_3D_inertia_tensors
    double v = 2.0 / 5.0 * mass * radius * radius;
    this->baseInertiaTensor = Matrix(v, 0, 0, 0, v, 0, 0, 0, v);
    this->invertedInertiaTensor = baseInertiaTensor.inverse();
    this->obb = OBB(pos, Point(radius, radius, radius), rotationMatrix);
}

double Ball::getRadius(){
    return radius;
}

void Ball::draw() {
    Color darkColor = Color(255, 255, 255);

    glPushMatrix();

    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixd(getOpenGLRotationMatrix());

    for(int i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j <= longs; j++)
        {
            if (j > longs / 2) glColor3ub( color.getR(), color.getG(), color.getB());
            else glColor3ub( darkColor.getR(), darkColor.getG(), darkColor.getB());
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            double s1, s2, t;
            s1 = ((double) i) / lats;
            s2 = ((double) i + 1) / lats;
            t = ((double) j) / longs;

            glNormal3d(x * zr0, y * zr0, z0);
            glVertex3d(radius *x * zr0, radius *y * zr0, radius *z0);

            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(radius *x * zr1, radius * y * zr1, radius *z1);
        }
        glEnd();

    }
    glPopMatrix();
 }

Matrix Ball::getInertiaTensorInverse() {
    return invertedInertiaTensor;
}