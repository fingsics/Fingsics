#include "../include/Room.h"

using namespace std;

Room::Room(float height, float floor, float leftWall, float rightWall, float backWall, float frontWall) {
    this->floor = floor;
    this->leftWall = leftWall;
    this->rightWall = rightWall;
    this->backWall = backWall;
    this->frontWall = frontWall;
    this->height = height;
    drawFrontWall = false;
    drawBackWall = true;
    drawLeftWall = true;
    drawRightWall = true;
}

Room::Room() {
    floor = 0;
    leftWall = -3;
    rightWall = 3;
    backWall = 3;
    frontWall = -3;
    height = 3;
    drawFrontWall = false;
    drawBackWall = true;
    drawLeftWall = true;
    drawRightWall = true;
}

float Room::getFloor() {
    return floor;
}

float Room::getLeftWall() {
    return leftWall;
}

float Room::getRightWall() {
    return rightWall;
}

float Room::getFrontWall() {
    return frontWall;
}

float Room::getBackWall() {
    return backWall;
}

void Room::draw() {
    // Floor
    glColor3ub(70, 70, 70);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(backWall, floor, leftWall);
    glVertex3f(frontWall, floor, leftWall);
    glVertex3f(frontWall, floor, rightWall);
    glVertex3f(backWall, floor, rightWall);
    glEnd();

    // Back wall
    if (drawBackWall) {
        glColor3ub(0, 0, 220);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(backWall, floor, leftWall);
        glVertex3f(backWall, floor, rightWall);
        glVertex3f(backWall, floor + height, rightWall);
        glVertex3f(backWall, floor + height, leftWall);
        glEnd();
    }

    // Front wall
    if (drawFrontWall) {
        glColor3ub(220, 220, 0);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(frontWall, floor, leftWall);
        glVertex3f(frontWall, floor, rightWall);
        glVertex3f(frontWall, floor + height, rightWall);
        glVertex3f(frontWall, floor + height, leftWall);
        glEnd();
    }

    // Left wall
    if (drawLeftWall) {
        glColor3ub(0, 220, 0);
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(backWall, floor + height, leftWall);
        glVertex3f(backWall, floor, leftWall);
        glVertex3f(frontWall, floor, leftWall);
        glVertex3f(frontWall, floor + height, leftWall);
        glEnd();
    }

    // Right wall
    if (drawRightWall) {
        glColor3ub(220, 0, 0);
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(backWall, floor, rightWall);
        glVertex3f(backWall, floor + height, rightWall);
        glVertex3f(frontWall, floor + height, rightWall);
        glVertex3f(frontWall, floor, rightWall);
        glEnd();
    }
}