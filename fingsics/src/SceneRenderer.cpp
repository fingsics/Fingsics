#include "../include/SceneRenderer.h";

SceneRenderer::SceneRenderer(vector<Object*> objects, int lats, int longs) {
    this->ballArrayLength = ((lats + 1) * (longs + 1)) * 2;
    this->capsuleArrayLength = ((lats + 1) * (longs + 1) + (longs + 1)) * 2;
    this->tileArrayLength = 4;

    int rocketTipRings = round((float)lats * 0.28);
    this->rocketArrayLength1 = ((rocketTipRings) * (longs + 1)) * 2;
    this->rocketArrayLength2 = ((lats + 1 - rocketTipRings) * (longs + 1)) * 2;

    for (int i = 0; i < objects.size(); i++) {
        if (Ball* ball = dynamic_cast<Ball*>(objects[i])) initializeBallArrays(ball, lats, longs);
        else if (Capsule* capsule = dynamic_cast<Capsule*>(objects[i])) {
            if (capsule->getDrawRocket()) initializeRocketArrays(capsule, lats, longs);
            else initializeCapsuleArrays(capsule, lats, longs);
        }
        else if (Tile* tile = dynamic_cast<Tile*>(objects[i])) initializeTileArrays(tile);
    }
}

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
    Vector dimensions = object->getOBB().getHalfLengths() * 2;
    Vector pos = object->getOBB().getPosition();

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

int SceneRenderer::openGLArrayLength(Object* object) {
    if (dynamic_cast<Ball*>(object)) return ballArrayLength;
    else if (dynamic_cast<Capsule*>(object)) return capsuleArrayLength;
    else if (dynamic_cast<Tile*>(object)) return tileArrayLength;
    return 0;
}

void SceneRenderer::drawRocket(Capsule* capsule) {
    glPushMatrix();
    glTranslatef(capsule->getPosition().getX(), capsule->getPosition().getY(), capsule->getPosition().getZ());
    glMultMatrixf(capsule->getRotationMatrix().getOpenGLRotationMatrix());

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_LIGHTING);
    glColor3ub(220, 0, 0);
    glVertexPointer(3, GL_FLOAT, 0, openGLVertices[capsule->getId()]);
    glDrawArrays(GL_QUAD_STRIP, 0, rocketArrayLength1);
    glColor3ub(65, 65, 65);
    glVertexPointer(3, GL_FLOAT, 0, openGLVertices[capsule->getId()] + rocketArrayLength1 * 3);
    glDrawArrays(GL_QUAD_STRIP, 0, rocketArrayLength2);
    glEnable(GL_LIGHTING);
    glColor3ub(251, 163, 26);
    glDisableClientState(GL_VERTEX_ARRAY);

    float fireHeight1 = 0.5;
    float fireHeight2 = 0.9;

    Vector up = Vector(0, 0, (capsule->getRadius() * 0.2) + capsule->getLength() / 2);
    Vector down = Vector(0, 0, (capsule->getRadius() * 0.2) + capsule->getLength() / 2 + capsule->getRadius() * (fireHeight2 + fireHeight1));
    Vector side1 = Vector(capsule->getRadius() * fireHeight1, capsule->getRadius() * fireHeight1, (capsule->getRadius() * 0.2) + capsule->getLength() / 2 + (capsule->getRadius() * fireHeight1));
    Vector side2 = Vector(capsule->getRadius() * fireHeight1, -capsule->getRadius() * fireHeight1, (capsule->getRadius() * 0.2) + capsule->getLength() / 2 + (capsule->getRadius() * fireHeight1));
    Vector side3 = Vector(-capsule->getRadius() * fireHeight1, -capsule->getRadius() * fireHeight1, (capsule->getRadius() * 0.2) + capsule->getLength() / 2 + (capsule->getRadius() * fireHeight1));
    Vector side4 = Vector(-capsule->getRadius() * fireHeight1, capsule->getRadius() * fireHeight1, (capsule->getRadius() * 0.2) + capsule->getLength() / 2 + (capsule->getRadius() * fireHeight1));

    Vector normal1 = (side2 - up).crossProduct(side1 - up);
    Vector normal2 = (side3 - up).crossProduct(side2 - up);
    Vector normal3 = (side4 - up).crossProduct(side3 - up);
    Vector normal4 = (side1 - up).crossProduct(side4 - up);
    Vector normal5 = (side1 - down).crossProduct(side2 - down);
    Vector normal6 = (side2 - down).crossProduct(side3 - down);
    Vector normal7 = (side3 - down).crossProduct(side4 - down);
    Vector normal8 = (side4 - down).crossProduct(side1 - down);

    glBegin(GL_TRIANGLES);
    glVertex3f(up.getX(), up.getY(), up.getZ());
    glVertex3f(side1.getX(), side1.getY(), side1.getZ());
    glVertex3f(side2.getX(), side2.getY(), side2.getZ());
    glNormal3f(normal1.getX(), normal1.getY(), normal1.getZ());
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(up.getX(), up.getY(), up.getZ());
    glVertex3f(side2.getX(), side2.getY(), side2.getZ());
    glVertex3f(side3.getX(), side3.getY(), side3.getZ());
    glNormal3f(normal2.getX(), normal2.getY(), normal2.getZ());
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(up.getX(), up.getY(), up.getZ());
    glVertex3f(side3.getX(), side3.getY(), side3.getZ());
    glVertex3f(side4.getX(), side4.getY(), side4.getZ());
    glNormal3f(normal3.getX(), normal3.getY(), normal3.getZ());
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(up.getX(), up.getY(), up.getZ());
    glVertex3f(side4.getX(), side4.getY(), side4.getZ());
    glVertex3f(side1.getX(), side1.getY(), side1.getZ());
    glNormal3f(normal4.getX(), normal4.getY(), normal4.getZ());
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(down.getX(), down.getY(), down.getZ());
    glVertex3f(side1.getX(), side1.getY(), side1.getZ());
    glVertex3f(side2.getX(), side2.getY(), side2.getZ());
    glNormal3f(normal5.getX(), normal5.getY(), normal5.getZ());
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(down.getX(), down.getY(), down.getZ());
    glVertex3f(side2.getX(), side2.getY(), side2.getZ());
    glVertex3f(side3.getX(), side3.getY(), side3.getZ());
    glNormal3f(normal6.getX(), normal6.getY(), normal6.getZ());
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(down.getX(), down.getY(), down.getZ());
    glVertex3f(side3.getX(), side3.getY(), side3.getZ());
    glVertex3f(side4.getX(), side4.getY(), side4.getZ());
    glNormal3f(normal7.getX(), normal7.getY(), normal7.getZ());
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(down.getX(), down.getY(), down.getZ());
    glVertex3f(side4.getX(), side4.getY(), side4.getZ());
    glVertex3f(side1.getX(), side1.getY(), side1.getZ());
    glNormal3f(normal8.getX(), normal8.getY(), normal8.getZ());
    glEnd();

    glColor3ub(0, 0, 220);

    glBegin(GL_TRIANGLES);
    glVertex3f(0, 0, -capsule->getLength() / 3);
    glVertex3f(0, capsule->getRadius() * 1.7, capsule->getLength() / 2);
    glVertex3f(0, -capsule->getRadius() * 1.7, capsule->getLength() / 2);
    glNormal3f(1, 0, 0);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(0, 0, -capsule->getLength() / 3);
    glVertex3f(capsule->getRadius() * 1.7, 0, capsule->getLength() / 2);
    glVertex3f(-capsule->getRadius() * 1.7, 0, capsule->getLength() / 2);
    glNormal3f(1, 0, 0);
    glEnd();

    glPopMatrix();
}

void SceneRenderer::drawObject(Object* object, bool drawOBB, bool drawAABB) {
    if (!object->getDraw()) return;

    if (drawOBB && !dynamic_cast<Tile*>(object)) this->drawOBB(object);
    if (drawAABB) this->drawAABB(object);

    Capsule* capsule = dynamic_cast<Capsule*>(object);
    if (capsule && capsule->getDrawRocket()) {
        drawRocket(capsule);
        return;
    }

    glPushMatrix();
    glTranslatef(object->getPosition().getX(), object->getPosition().getY(), object->getPosition().getZ());
    glMultMatrixf(object->getRotationMatrix().getOpenGLRotationMatrix());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, openGLVertices[object->getId()]);
    glNormalPointer(GL_FLOAT, 0, openGLNormals[object->getId()]);
    
    glColor3ub(object->getColor().getR(), object->getColor().getG(), object->getColor().getB());
    glDrawArrays(GL_QUAD_STRIP, 0, openGLArrayLength(object));
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glPopMatrix();
}

void SceneRenderer::initializeBallArrays(Ball* ball, int lats, int longs) {
    float radius = ball->getRadius();
    int arrayLength = ((lats + 1) * (longs + 1)) * 2 * 3;
    float* vertices = new float[arrayLength];
    float* normals = new float[arrayLength];

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

            normals[arrayIndex] = x * zr0;
            normals[arrayIndex + 1] = y * zr0;
            normals[arrayIndex + 2] = z0;

            vertices[arrayIndex] = radius * x * zr0;
            vertices[arrayIndex + 1] = radius * y * zr0;
            vertices[arrayIndex + 2] = radius * z0;

            normals[arrayIndex + 3] = x * zr1;
            normals[arrayIndex + 4] = y * zr1;
            normals[arrayIndex + 5] = z1;

            vertices[arrayIndex + 3] = radius * x * zr1;
            vertices[arrayIndex + 4] = radius * y * zr1;
            vertices[arrayIndex + 5] = radius * z1;

            arrayIndex += 6;
        }
    }

    this->openGLVertices[ball->getId()] = vertices;
    this->openGLNormals[ball->getId()] = normals;
}

void SceneRenderer::initializeRocketArrays(Capsule* capsule, int lats, int longs) {
    float radius = capsule->getRadius();
    float length = capsule->getLength();
    
    int arrayLength = ((lats + 1) * (longs + 1)) * 2 * 3;
    float* vertices = new float[arrayLength];
    float* normals = new float[arrayLength];

    int arrayIndex = 0;

    for (int i = 0; i <= lats; i++) {
        float lat0 = M_PI * (-0.5 + (float)(i - 1) / lats);
        float zr0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float)i / lats);
        float zr1 = cos(lat1);

        if (i < lats / 2 + 1) {
            float z0 = sin(lat0) * (length + radius);
            float z1 = sin(lat1) * (length + radius);
            for (int j = 0; j <= longs; j++) {

                float lng = 2 * M_PI * (float)(j - 1) / longs;
                float x = cos(lng);
                float y = sin(lng);

                float s1 = ((float)i) / longs;
                float s2 = ((float)i + 1) / lats;
                float t = ((float)j) / lats;

                vertices[arrayIndex] = radius * x * zr0;
                vertices[arrayIndex + 1] = radius * y * zr0;
                vertices[arrayIndex + 2] = z0 + length / 2;

                vertices[arrayIndex + 3] = radius * x * zr1;
                vertices[arrayIndex + 4] = radius * y * zr1;
                vertices[arrayIndex + 5] = z1 + length / 2;

                arrayIndex += 6;
            }
        }
        else {
            float z0 = sin(lat0) * radius * 0.2;
            float z1 = sin(lat1) * radius * 0.2;
            for (int j = 0; j <= longs; j++) {
                float lng = 2 * M_PI * (float)(j - 1) / longs;
                float x = cos(lng);
                float y = sin(lng);

                float s1 = ((float)i) / longs;
                float s2 = ((float)i + 1) / lats;
                float t = ((float)j) / lats;

                vertices[arrayIndex] = radius * x * zr0;
                vertices[arrayIndex + 1] = radius * y * zr0;
                vertices[arrayIndex + 2] = z0 + length / 2;

                vertices[arrayIndex + 3] = radius * x * zr1;
                vertices[arrayIndex + 4] = radius * y * zr1;
                vertices[arrayIndex + 5] = z1 + length / 2;

                arrayIndex += 6;
            }
        }
        
    }

    this->openGLVertices[capsule->getId()] = vertices;
    this->openGLNormals[capsule->getId()] = normals;
}

void SceneRenderer::initializeCapsuleArrays(Capsule* capsule, int lats, int longs) {
    float radius = capsule->getRadius();
    float length = capsule->getLength();
    int arrayLength = ((lats + 1) * (longs + 1) + (longs + 1)) * 2 * 3;
    float* vertices = new float[arrayLength];
    float* normals = new float[arrayLength];

    int arrayIndex = 0;
    float zDisplacement = -length / 2.0;

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

            normals[arrayIndex] = x * zr0;
            normals[arrayIndex + 1] = y * zr0;
            normals[arrayIndex + 2] = z0;

            vertices[arrayIndex] = radius * x * zr0;
            vertices[arrayIndex + 1] = radius * y * zr0;
            vertices[arrayIndex + 2] = radius * z0 + zDisplacement;

            normals[arrayIndex + 3] = x * zr1;
            normals[arrayIndex + 4] = y * zr1;
            normals[arrayIndex + 5] = z1;

            vertices[arrayIndex + 3] = radius * x * zr1;
            vertices[arrayIndex + 4] = radius * y * zr1;
            vertices[arrayIndex + 5] = radius * z1 + zDisplacement;

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

                normals[arrayIndex] = x * zr1;
                normals[arrayIndex + 1] = y * zr1;
                normals[arrayIndex + 2] = z1;

                vertices[arrayIndex] = radius * x * zr1;
                vertices[arrayIndex + 1] = radius * y * zr1;
                vertices[arrayIndex + 2] = radius * z1 + zDisplacement;

                normals[arrayIndex + 3] = x * zr1;
                normals[arrayIndex + 4] = y * zr1;
                normals[arrayIndex + 5] = z1;

                vertices[arrayIndex + 3] = radius * x * zr1;
                vertices[arrayIndex + 4] = radius * y * zr1;
                vertices[arrayIndex + 5] = radius * z1 + length + zDisplacement;

                arrayIndex += 6;
            }
            zDisplacement += length;
        }
    }

    this->openGLVertices[capsule->getId()] = vertices;
    this->openGLNormals[capsule->getId()] = normals;
}

void SceneRenderer::initializeTileArrays(Tile* tile) {
    float axis1Length = tile->getAxis1Length() / 2.0;
    float axis2Length = tile->getAxis2Length() / 2.0;
    int arrayLength = 12;
    float* vertices = new float[12];
    vertices[0] = -axis1Length;
    vertices[1] = 0;
    vertices[2] = axis2Length;
    vertices[3] = axis1Length;
    vertices[4] = 0;
    vertices[5] = axis2Length;
    vertices[6] = -axis1Length;
    vertices[7] = 0;
    vertices[8] = -axis2Length;
    vertices[9] = axis1Length;
    vertices[10] = 0;
    vertices[11] = -axis2Length;
    float* normals = new float[12];
    normals[0] = 0;
    normals[1] = 1;
    normals[2] = 0;
    normals[3] = 0;
    normals[4] = 1;
    normals[5] = 0;
    normals[6] = 0;
    normals[7] = 1;
    normals[8] = 0;
    normals[9] = 0;
    normals[10] = 1;
    normals[11] = 0;
    this->openGLVertices[tile->getId()] = vertices;
    this->openGLNormals[tile->getId()] = normals;
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