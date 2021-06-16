#include "../include/Ball.h"

using namespace std;

Ball::Ball(string id, Point pos, Point vel, Point force, double rad, double mass, Color color) :  Object(id, pos, vel, force, mass, color) {
    this->rad = rad;
}

double Ball::getRad(){
    return rad;
}

void Ball::draw() {
    glPushMatrix();

    // Ball movement
    glTranslatef(pos.getX(), pos.getY(), pos.getZ());

    // Draw sphere
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
            glVertex3d(rad*x * zr0, rad*y * zr0, rad*z0);

            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(rad*x * zr1,rad* y * zr1, rad*z1);
        }
        glEnd();

    }
    glPopMatrix();
 }
