#include "../include/SceneRenderer.h";

void SceneRenderer::initializeOpenGL(int resolutionWidth, int resolutionHeight) {
    glMatrixMode(GL_PROJECTION);
    glClearColor(0, 0, 0, 1);
    gluPerspective(45, (float)resolutionWidth / (float)resolutionHeight, 0.1, 500);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
}

void SceneRenderer::setupFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void SceneRenderer::drawAxis() {
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

void SceneRenderer::setLighting() {
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

void SceneRenderer::drawOBB(Object* object) {
    Point dimensions = object->getOBB().getHalfLengths() * 2;
    Point pos = object->getOBB().getPosition();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3ub(220, 220, 0);
    glDisable(GL_LIGHTING);

    glPushMatrix();

    glTranslatef(pos.getX(), pos.getY(), pos.getZ());
    glMultMatrixf(object->getRotationMatrix().getOpenGLRotationMatrix());
    glTranslatef(-dimensions.getX() / 2.0, -dimensions.getY() / 2.0, -dimensions.getZ() / 2.0);

    glBegin(GL_QUAD_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, dimensions.getZ());
    glVertex3f(0, dimensions.getY(), 0);
    glVertex3f(0, dimensions.getY(), dimensions.getZ());
    glVertex3f(dimensions.getX(), dimensions.getY(), 0);
    glVertex3f(dimensions.getX(), dimensions.getY(), dimensions.getZ());
    glVertex3f(dimensions.getX(), 0, 0);
    glVertex3f(dimensions.getX(), 0, dimensions.getZ());
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, dimensions.getZ());
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SceneRenderer::drawAABB(Object* object) {
    float minX = object->getMinX();
    float maxX = object->getMaxX();
    float minY = object->getMinY();
    float maxY = object->getMaxY();
    float minZ = object->getMinZ();
    float maxZ = object->getMaxZ();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3ub(220, 0, 220);
    glDisable(GL_LIGHTING);

    glBegin(GL_QUAD_STRIP);
    glVertex3f(minX, minY, minZ);
    glVertex3f(maxX, minY, minZ);
    glVertex3f(minX, maxY, minZ);
    glVertex3f(maxX, maxY, minZ);
    glVertex3f(minX, maxY, maxZ);
    glVertex3f(maxX, maxY, maxZ);
    glVertex3f(minX, minY, maxZ);
    glVertex3f(maxX, minY, maxZ);
    glVertex3f(minX, minY, minZ);
    glVertex3f(maxX, minY, minZ);
    glEnd();

    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void SceneRenderer::drawObject(Object* object, bool drawOBB, bool drawAABB) {
    if (drawOBB && !dynamic_cast<Tile*>(object)) this->drawOBB(object);
    if (drawAABB) this->drawAABB(object);

    Ball* ballCast = dynamic_cast<Ball*>(object);
    Tile* tileCast = dynamic_cast<Tile*>(object);
    Capsule* capsuleCast = dynamic_cast<Capsule*>(object);

    if (ballCast) drawBall(ballCast);
    else if (tileCast) drawTile(tileCast);
    else if (capsuleCast) drawCapsule(capsuleCast);
}

void SceneRenderer::drawBall(Ball* ball) {
    glPushMatrix();

    Color color = ball->getColor();
    Point position = ball->getPosition();
    float radius = ball->getRadius();
    Matrix rotationMatrix = ball->getRotationMatrix();
    int lats = ball->getLats();
    int longs = ball->getLongs();

    glTranslatef(position.getX(), position.getY(), position.getZ());
    glMultMatrixf(rotationMatrix.getOpenGLRotationMatrix());

    int arrayIndex = 0;

    for (int i = 0; i <= lats; i++) {
        float lat0 = M_PI * (-0.5 + (float)(i - 1) / lats);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float)i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        for (int j = 0; j <= longs; j++) {
            float lng = 2 * M_PI * (float)(j - 1) / longs;
            float x = cos(lng);
            float y = sin(lng);

            float s1, s2, t;
            s1 = ((float)i) / lats;
            s2 = ((float)i + 1) / lats;
            t = ((float)j) / longs;

            openGLNormals[arrayIndex] = x * zr0;
            openGLNormals[arrayIndex + 1] = y * zr0;
            openGLNormals[arrayIndex + 2] = z0;

            openGLVertices[arrayIndex] = radius * x * zr0;
            openGLVertices[arrayIndex + 1] = radius * y * zr0;
            openGLVertices[arrayIndex + 2] = radius * z0;

            openGLNormals[arrayIndex + 3] = x * zr1;
            openGLNormals[arrayIndex + 4] = y * zr1;
            openGLNormals[arrayIndex + 5] = z1;

            openGLVertices[arrayIndex + 3] = radius * x * zr1;
            openGLVertices[arrayIndex + 4] = radius * y * zr1;
            openGLVertices[arrayIndex + 5] = radius * z1;

            arrayIndex += 6;
        }
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, openGLVertices);
    glNormalPointer(GL_FLOAT, 0, openGLNormals);
    glColor3ub(color.getR(), color.getG(), color.getB());
    glDrawArrays(GL_QUAD_STRIP, 0, openGLArrayLength / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopMatrix();
}

void SceneRenderer::drawTile(Tile* tile) {
    if (!tile->getDraw()) return;
    Color color = tile->getColor();
    Point position = tile->getPosition();
    Matrix rotationMatrix = tile->getRotationMatrix();
    float axis1Length = tile->getAxis1Length();
    float axis2Length = tile->getAxis2Length();

    glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glMultMatrixf(rotationMatrix.getOpenGLRotationMatrix());
    glColor3ub(color.getR(), color.getG(), color.getB());
    glBegin(GL_QUADS);
    glVertex3d(-axis1Length / 2.0, 0, axis2Length / 2.0);
    glVertex3d(axis1Length / 2.0, 0, axis2Length / 2.0);
    glVertex3d(axis1Length / 2.0, 0, -axis2Length / 2.0);
    glVertex3d(-axis1Length / 2.0, 0, -axis2Length / 2.0);
    glEnd();
    glPopMatrix();
}

void SceneRenderer::drawCapsule(Capsule* capsule) {
    glPushMatrix();

    Color color = capsule->getColor();
    Point position = capsule->getPosition();
    float radius = capsule->getRadius();
    float length = capsule->getLength();
    Matrix rotationMatrix = capsule->getRotationMatrix();
    int lats = capsule->getLats();
    int longs = capsule->getLongs();

    glColor3ub(color.getR(), color.getG(), color.getB());
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glMultMatrixf(rotationMatrix.getOpenGLRotationMatrix());
    glTranslatef(0, 0, -length / 2.0);

    int arrayIndex = 0;
    float zDisplacement = 0;

    for (int i = 0; i <= lats; i++) {
        float lat0 = M_PI * (-0.5 + (float)(i - 1) / lats);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float)i / lats);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        for (int j = 0; j <= longs; j++) {

            float lng = 2 * M_PI * (float)(j - 1) / longs;
            float x = cos(lng);
            float y = sin(lng);

            float s1 = ((float)i) / longs;
            float s2 = ((float)i + 1) / lats;
            float t = ((float)j) / lats;

            openGLNormals[arrayIndex] = x * zr0;
            openGLNormals[arrayIndex + 1] = y * zr0;
            openGLNormals[arrayIndex + 2] = z0;

            openGLVertices[arrayIndex] = radius * x * zr0;
            openGLVertices[arrayIndex + 1] = radius * y * zr0;
            openGLVertices[arrayIndex + 2] = radius * z0 + zDisplacement;

            openGLNormals[arrayIndex + 3] = x * zr1;
            openGLNormals[arrayIndex + 4] = y * zr1;
            openGLNormals[arrayIndex + 5] = z1;

            openGLVertices[arrayIndex + 3] = radius * x * zr1;
            openGLVertices[arrayIndex + 4] = radius * y * zr1;
            openGLVertices[arrayIndex + 5] = radius * z1 + zDisplacement;

            arrayIndex += 6;
        }

        if (i == lats / 2) {
            for (int j = 0; j <= longs; j++) {
                float lng = 2 * M_PI * (float)(j - 1) / longs;
                float x = cos(lng);
                float y = sin(lng);

                float s1 = ((float)i) / lats;
                float s2 = ((float)i + 1) / lats;
                float t = ((float)j) / longs;

                openGLNormals[arrayIndex] = x * zr1;
                openGLNormals[arrayIndex + 1] = y * zr1;
                openGLNormals[arrayIndex + 2] = z1;

                openGLVertices[arrayIndex] = radius * x * zr1;
                openGLVertices[arrayIndex + 1] = radius * y * zr1;
                openGLVertices[arrayIndex + 2] = radius * z1;

                openGLNormals[arrayIndex + 3] = x * zr1;
                openGLNormals[arrayIndex + 4] = y * zr1;
                openGLNormals[arrayIndex + 5] = z1;

                openGLVertices[arrayIndex + 3] = radius * x * zr1;
                openGLVertices[arrayIndex + 4] = radius * y * zr1;
                openGLVertices[arrayIndex + 5] = radius * z1 + length;

                arrayIndex += 6;
            }
            zDisplacement = length;
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, openGLVertices);
    glNormalPointer(GL_FLOAT, 0, openGLNormals);
    glColor3ub(color.getR(), color.getG(), color.getB());
    glDrawArrays(GL_QUAD_STRIP, 0, openGLArrayLength / 3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopMatrix();
}

void SceneRenderer::drawF(float up, float left, float height, float width, float mid) {
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

void SceneRenderer::drawP(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up + height);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glEnd();
}

void SceneRenderer::drawS(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up);
    glVertex2f(left, up);
    glVertex2f(left, up + height * mid);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left + width, up + height);
    glVertex2f(left, up + height);
    glEnd();
}

void SceneRenderer::draw0(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height);
    glVertex2f(left, up + height);
    glVertex2f(left, up);
    glEnd();
}

void SceneRenderer::draw1(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width * 0.7, up);
    glVertex2f(left + width * 0.7, up + height);
    glEnd();
}

void SceneRenderer::draw2(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glVertex2f(left, up + height);
    glVertex2f(left + width, up + height);
    glEnd();
}

void SceneRenderer::draw3(float up, float left, float height, float width, float mid) {
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

void SceneRenderer::draw4(float up, float left, float height, float width, float mid) {
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

void SceneRenderer::draw5(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up);
    glVertex2f(left, up);
    glVertex2f(left, up + height * mid);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left + width, up + height);
    glVertex2f(left, up + height);
    glEnd();
}

void SceneRenderer::draw6(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up);
    glVertex2f(left, up);
    glVertex2f(left, up + height);
    glVertex2f(left + width, up + height);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glEnd();
}

void SceneRenderer::draw7(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height);
    glEnd();
}

void SceneRenderer::draw8(float up, float left, float height, float width, float mid) {
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

void SceneRenderer::draw9(float up, float left, float height, float width, float mid) {
    glBegin(GL_LINE_STRIP);
    glVertex2f(left + width, up + height * mid);
    glVertex2f(left, up + height * mid);
    glVertex2f(left, up);
    glVertex2f(left + width, up);
    glVertex2f(left + width, up + height);
    glEnd();
}

void SceneRenderer::drawCharacter(char character, float up, float left, float height, float width, float mid) {
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

void SceneRenderer::drawFPSCounter(int fps) {
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