#include "../include/Box.h"

using namespace std;

Box::Box(string id, Point pos, Point vel, Point force, Point dimensions, Point angle, Point angularVelocity, double mass, double elasticityCoef, Color color) :  Object(id, pos, vel, force, mass, elasticityCoef, color) {
    this->dimensions = dimensions;
    this->angle = angle;
    this->angularVelocity = angularVelocity;
}

Point Box::getDimensions(){
    return dimensions;
}

void Box::draw() {
    glPushMatrix();

    glTranslatef(pos.getX() - dimensions.getX() / 2, pos.getY() - dimensions.getY() / 2, pos.getZ() - dimensions.getZ() / 2);
    glColor3ub(color.getR(), color.getG(), color.getB());

    glBegin(GL_QUAD_STRIP);
    glVertex3f(0,0,0);
    glVertex3f(0,0,dimensions.getZ());
    glVertex3f(0,dimensions.getY(),0);
    glVertex3f(0,dimensions.getY(),dimensions.getZ());
    glVertex3f(dimensions.getX(),dimensions.getY(),0);
    glVertex3f(dimensions.getX(),dimensions.getY(),dimensions.getZ());
    glVertex3f(dimensions.getX(),0,0);
    glVertex3f(dimensions.getX(),0,dimensions.getZ());
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, dimensions.getZ());
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(0, dimensions.getY(), 0);
    glVertex3f(dimensions.getX(), dimensions.getY(), 0);
    glVertex3f(dimensions.getX(), 0, 0);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(0, 0, dimensions.getZ());
    glVertex3f(0, dimensions.getY(), dimensions.getZ());
    glVertex3f(dimensions.getX(), dimensions.getY(), dimensions.getZ());
    glVertex3f(dimensions.getX(), 0, dimensions.getZ());
    glEnd();

    glPopMatrix();
 }

void Box::updatePosAndVel(double secondsElapsed) {
    // Check collision with floor

    // Check collision with walls

    // Update velocity
    Point acceleration = force / mass;
    vel = vel + acceleration * secondsElapsed;

    // Update position
    double epsilon = 0.001;
    pos = Point(pos.getX() + vel.getX() * secondsElapsed, max(dimensions.getY() / 2 - epsilon, pos.getY() + vel.getY() * secondsElapsed), pos.getZ() + vel.getZ() * secondsElapsed);
    
    // Update angular velocity

    // Update angle
}