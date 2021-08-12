#include "../include/OpenGLHelpers.h";

void initializeOpenGL() {
    glMatrixMode(GL_PROJECTION);
    glClearColor(0, 0, 0, 1);
    gluPerspective(45, 1280 / 720.f, 0.1, 100);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
}

void setupFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void drawObjects(Object** objects, int numObjects, bool drawOBBs, bool drawAABBs, bool drawHalfWhite) {
    for (int i = 0; i < numObjects; i++) objects[i]->draw(drawOBBs, drawAABBs, drawHalfWhite);
}

void drawAxis() {
    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);
    glEnd();
    glColor3ub(0, 255, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);
    glEnd();
    glColor3ub(0, 0, 255);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
    glEnd();
}

void setLighting() {
    float lightR = 1.0f;
    float lightG = 1.0f;
    float lightB = 1.0f;

    GLfloat position[] = { 0, 5, 0, 1 };
    GLfloat ambient[] = { lightR / 10, lightG / 10, lightB / 10, 0.4f };
    GLfloat specular[] = { lightR, lightG, lightB, 2.0f };
    GLfloat diffuse[] = { lightR, lightG, lightB, 2.0f };
    GLfloat direction[] = { 0, -1, 0 };

    GLfloat materialAmbient[] = { .2, .2, .2, 1 };
    GLfloat materialDiffuse[] = { .8, .8, .8, 1 };

    glPushMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);

    glPopMatrix();
}

void drawF(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up + height);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glEnd();
}

void drawP(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up + height);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glEnd();
}

void drawS(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up);
    glVertex2f(left, up);
    glVertex2f(left, up + height * mid);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left + width, up + height);
    glVertex2f(left, up + height);
    glEnd();
}

void draw0(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height);
    glVertex2f(left, up + height);
    glVertex2f(left, up);
    glEnd();
}

void draw1(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width * 0.7, up);
    glVertex2f(left + width * 0.7, up + height);
    glEnd();
}

void draw2(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glVertex2f(left, up + height);
    glVertex2f(left + width, up + height);
    glEnd();
}

void draw3(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left + width, up + height);
    glVertex2f(left, up + height);
    glEnd();
}

void draw4(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left, up + height * mid);
    glVertex2f(left + width, up + height * mid);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height);
    glEnd();
}

void draw5(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glVertex2f(left, up + height);
    glVertex2f(left + width, up + height);
    glEnd();
}

void draw6(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up);
    glVertex2f(left, up);
    glVertex2f(left, up + height);
    glVertex2f(left + width, up + height);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glEnd();
}

void draw7(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height);
    glEnd();
}

void draw8(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up + height * mid);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glVertex2f(left, up + height);
    glVertex2f(left + width, up + height);
    glVertex2f(left + width, up + height * mid);
    glEnd();
}

void draw9(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height);
    glEnd();
}

void drawFPSCounter() {
    float up = 0.02;
    float left = 0.02;

    float letterHeight = 0.02;
    float letterWidth = letterHeight * 0.5;
    float lineWidth = 2;
    float midRatio = 0.5;
    float letterSpace = letterWidth * 1.3;

    float width = 0.3;
    float height = letterHeight * 1.4;
    float letterUp = up + (height - letterHeight) / 2;
    float letterLeft = letterUp;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);

    glColor3ub(170, 170, 170);

    glLineWidth(lineWidth);
    drawF(letterUp, letterLeft, letterHeight, letterWidth, midRatio);
    drawP(letterUp, letterLeft + letterSpace, letterHeight, letterWidth, midRatio);
    drawS(letterUp, letterLeft + letterSpace + letterSpace, letterHeight, letterWidth, midRatio);
    glLineWidth(1);

    glBegin(GL_QUADS);
    glColor3ub(30, 30, 30);
    glVertex2f(left, up);
    glVertex2f(left, up + height);
    glVertex2f(left + width, up + height);
    glVertex2f(left + width, up);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}
