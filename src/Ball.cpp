#include "../include/Ball.h"

using namespace std;

Ball::Ball(string id, Point pos, Point vel, Point force, Point angle, Point angularVelocity, double radius, double mass, double elasticityCoef, Color color) :  Object(id, pos, vel, force, angle, angularVelocity, mass, elasticityCoef, color) {
    this->radius = radius;
}

double Ball::getRadius(){
    return radius;
}

void Ball::draw() {
    glPushMatrix();
    glTranslatef(pos.getX(), pos.getY(), pos.getZ());
    // TODO: ROTATE PROPERLY
    glRotatef(angle.getX(), 1, 0, 0);
    glRotatef(angle.getY(), 0, 1, 0);
    glRotatef(angle.getZ(), 0, 0, 1);

    for(int i = 0; i <= LATS; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / LATS);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / LATS);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        glColor3ub( color.getR(), color.getG(), color.getB());
        for(int j = 0; j <= LONGS; j++)
        {
            double lng = 2 * M_PI * (double) (j - 1) / LONGS;
            double x = cos(lng);
            double y = sin(lng);

            double s1, s2, t;
            s1 = ((double) i) / LATS;
            s2 = ((double) i + 1) / LATS;
            t = ((double) j) / LONGS;

            glNormal3d(x * zr0, y * zr0, z0);
            glVertex3d(radius *x * zr0, radius *y * zr0, radius *z0);

            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(radius *x * zr1, radius * y * zr1, radius *z1);
        }
        glEnd();

    }
    glPopMatrix();
 }

Matrix Ball::getInertiaTensor() {
    // https://en.wikipedia.org/wiki/List_of_moments_of_inertia#List_of_3D_inertia_tensors
    double v = 2.0 / 5.0 * mass * radius * radius;
    return Matrix(Point(v, 0, 0),
                  Point(0, v, 0),
                  Point(0, 0, v));
}