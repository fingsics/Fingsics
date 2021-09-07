#include "../include/OpenGLHelpers.h";

void initializeOpenGL(int resolutionWidth, int resolutionHeight) {
    glMatrixMode(GL_PROJECTION);
    glClearColor(0, 0, 0, 1);
    gluPerspective(45, (float)resolutionWidth / (float)resolutionHeight, 0.1, 500);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
}

void setupFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
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
    glVertex2f(left + width, up);
    glVertex2f(left, up);
    glVertex2f(left, up + height * mid);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left + width, up + height);
    glVertex2f(left, up + height);
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

void drawCharacter(char character, float up, float left, float height, float width, float mid) {
    switch (tolower(character)) {
    case 'a': return;
    case 'b': return;
    case 'c': return;
    case 'd': return;
    case 'e': return;
    case 'f': drawF(up, left, height, width, mid); return;
    case 'g': return;
    case 'h': return;
    case 'i': return;
    case 'j': return;
    case 'k': return;
    case 'l': return;
    case 'm': return;
    case 'n': return;
    case 'o': return;
    case 'p': drawP(up, left, height, width, mid); return;
    case 'q': return;
    case 'r': return;
    case 's': drawS(up, left, height, width, mid); return;
    case 't': return;
    case 'u': return;
    case 'v': return;
    case 'w': return;
    case 'x': return;
    case 'y': return;
    case 'z': return;
    case '0': draw0(up, left, height, width, mid); return;
    case '1': draw1(up, left, height, width, mid); return;
    case '2': draw2(up, left, height, width, mid); return;
    case '3': draw3(up, left, height, width, mid); return;
    case '4': draw4(up, left, height, width, mid); return;
    case '5': draw5(up, left, height, width, mid); return;
    case '6': draw6(up, left, height, width, mid); return;
    case '7': draw7(up, left, height, width, mid); return;
    case '8': draw8(up, left, height, width, mid); return;
    case '9': draw9(up, left, height, width, mid); return;
    case '.': return;
    case ' ': return;
    case ':': return;
    }
}

void drawFPSCounter(int fps) {
    string stringFPS(to_string(fps) + " FPS");

    float up = 0.02;
    float left = 0.01;

    float letterHeight = 0.02;
    float letterWidth = letterHeight * 0.5;
    float lineWidth = 2;
    float midRatio = 0.5;
    float letterSpace = letterWidth * 1.3;

    float height = letterHeight * 1.4;
    float letterUp = (height - letterHeight) / 2;
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
    int n = 0;
    for (string::iterator it = stringFPS.begin(); it != stringFPS.end(); ++it) {
        drawCharacter(*it, up + letterUp, left + letterLeft + n * letterSpace, letterHeight, letterWidth, midRatio);
        n++;
    }
    glLineWidth(1);

    float width = height - letterHeight + letterSpace * n;

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