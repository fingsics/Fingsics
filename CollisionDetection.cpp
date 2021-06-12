// CollisionDetection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "include/Color.h"
#include "include/Point.h"
#include "include/Ball.h"
#include <thread>
#include <string>
#include <glm/glm.hpp>
#include <glut.h>

#define _USE_MATH_DEFINES
#define FPS 60
#define M_PI 3.1415926

using namespace std;

void updateCam(float& x, float& y, float& z, float x_angle, float y_angle, float radius) {
    z = cos(y_angle * M_PI / 180) * cos(x_angle * M_PI / 180) * radius;
    x = sin(y_angle * M_PI / 180) * cos(x_angle * M_PI / 180) * radius;
    y = sin(x_angle * M_PI / 180) * radius;
}

Ball** initializeBalls(double ballRad, double ballMass, float ballSeparation) {
    Ball** balls = new Ball * [16];
    Color* ballColor = new Color(200, 20, 20);
    float height = ballRad;
    float whiteBallDistance = 1.5;

    // White ball
    balls[0] = new Ball(-2, height, 0, ballRad, ballMass, ballColor);
    // First line
    balls[1] = new Ball(whiteBallDistance, height, 0, ballRad, ballMass, ballColor);
    // Second line
    balls[2] = new Ball(whiteBallDistance + ballSeparation, height, ballRad, ballRad, ballMass, ballColor);
    balls[3] = new Ball(whiteBallDistance + ballSeparation, height, -ballRad, ballRad, ballMass, ballColor);
    // Third line
    balls[4] = new Ball(whiteBallDistance + ballSeparation * 2, height, 0, ballRad, ballMass, ballColor);
    balls[5] = new Ball(whiteBallDistance + ballSeparation * 2, height, ballRad * 2, ballRad, ballMass, ballColor);
    balls[6] = new Ball(whiteBallDistance + ballSeparation * 2, height, -ballRad * 2, ballRad, ballMass, ballColor);
    // Forth line
    balls[7] = new Ball(whiteBallDistance + ballSeparation * 3, height, ballRad, ballRad, ballMass, ballColor);
    balls[8] = new Ball(whiteBallDistance + ballSeparation * 3, height, -ballRad, ballRad, ballMass, ballColor);
    balls[9] = new Ball(whiteBallDistance + ballSeparation * 3, height, ballRad + ballRad * 2, ballRad, ballMass, ballColor);
    balls[10] = new Ball(whiteBallDistance + ballSeparation * 3, height, -ballRad - ballRad * 2, ballRad, ballMass, ballColor);
    // Fifth line
    balls[11] = new Ball(whiteBallDistance + ballSeparation * 4, height, 0, ballRad, ballMass, ballColor);
    balls[12] = new Ball(whiteBallDistance + ballSeparation * 4, height, ballRad * 2, ballRad, ballMass, ballColor);
    balls[13] = new Ball(whiteBallDistance + ballSeparation * 4, height, ballRad * 4, ballRad, ballMass, ballColor);
    balls[14] = new Ball(whiteBallDistance + ballSeparation * 4, height, -ballRad * 2, ballRad, ballMass, ballColor);
    balls[15] = new Ball(whiteBallDistance + ballSeparation * 4, height, -ballRad * 4, ballRad, ballMass, ballColor);

    return balls;
}


void drawCue(int numSteps, float radius, float hl, Ball* whiteBall, float cueRotAng1, float cueRotAng2, float strength) {
    float a = 0.0f;
    float step = (2 * M_PI) / (float)numSteps;
    double ballRad = whiteBall->getRad();


    glPushMatrix();

    // Move the cue next to the white ball
    glTranslatef(whiteBall->getPosX(), whiteBall->getPosY(), whiteBall->getPosZ());

    // Rotate the cue based on the mouse movements
    glRotatef(cueRotAng1, 0, 1, 0);
    glRotatef(cueRotAng2, 1, 0, 0);

    // Initial cue position and rotation
    glTranslatef(0, hl + ballRad + strength / 4 * ballRad, 0);
    glRotatef(-90, 1, 0, 0);

    // Draw the cylinder
    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(152, 118, 84);
    for (int i = 0; i <= numSteps; i++)
    {
        float x = cos(a) * radius;
        float y = sin(a) * radius;
        glNormal3f(x / radius, y / radius, 0);
        glVertex3f(x, y, -hl);
        glNormal3f(x / radius, y / radius, 0);
        glVertex3f(x, y, hl);
        a += step;
    }
    glEnd();
    glPopMatrix();
}

void moveBalls(Ball** balls, float time, float tableLength, float tableWidth) {
    for (int i = 0; i < 16; i++) balls[i]->updatePosAndVel(time, tableLength, tableWidth, balls);
}

void hitBall(Ball* whiteBall, float cueRotAng1, float cueRotAng2, int strength) {

    float strengthFactor = 2;

    // Convert to radians
    cueRotAng1 = cueRotAng1 * M_PI / 180;
    cueRotAng2 = cueRotAng2 * M_PI / 180;

    // Initial cue position
    float x = 0;
    float y = 1;
    float z = 0;

    // Vertical cue rotation
    float rotatedY = cos(cueRotAng2) * y - sin(cueRotAng2) * z;
    float rotatedZ = sin(cueRotAng2) * y + cos(cueRotAng2) * z;

    // Horizontal cue rotation
    float rotatedX = cos(cueRotAng1) * x - sin(cueRotAng1) * rotatedZ;
    rotatedZ = sin(cueRotAng1) * x + cos(cueRotAng1) * rotatedZ;

    whiteBall->setVelocity(new Point(rotatedX * strength * strengthFactor, 0, -rotatedZ * strength * strengthFactor));
}


void applyCollision(Ball** balls, int ball1Idx, int ball2Idx, double ballRad, bool** colliding) {

    Ball* ball1 = balls[ball1Idx];
    Ball* ball2 = balls[ball2Idx];
    Point* ball1Pos = ball1->getPos();
    Point* ball2Pos = ball2->getPos();
    Point* ball1Vel = ball1->getVel();
    Point* ball2Vel = ball2->getVel();
    double ball1Mass = ball1->getMass();
    double ball2Mass = ball2->getMass();

    Point* normalVector = (*ball1Pos) - ball2Pos;
    double magnitude = normalVector->magnitude();

    if (magnitude < ballRad * 2 && !colliding[ball1Idx][ball2Idx]) {

        colliding[ball1Idx][ball2Idx] = true;
        colliding[ball2Idx][ball1Idx] = true;

        Point* unitVector = (*normalVector) / magnitude;
        Point* tangentVector = new Point(-unitVector->getZ(), -unitVector->getY(), unitVector->getX());

        double vector1NormalMagnitude = unitVector->dotProduct(ball1Vel);
        double vector1TangentMagnitude = tangentVector->dotProduct(ball1Vel);
        double vector2NormalMagnitude = unitVector->dotProduct(ball2Vel);
        double vector2TangentMagnitude = tangentVector->dotProduct(ball2Vel);

        // Because of conservation of kinetic energy
        double newVector1NormalMagnitude = (vector1NormalMagnitude * (ball1Mass - ball2Mass) + 2 * ball2Mass * vector2NormalMagnitude) / (ball1Mass + ball2Mass);
        double newVector2NormalMagnitude = (vector2NormalMagnitude * (ball2Mass - ball1Mass) + 2 * ball1Mass * vector1NormalMagnitude) / (ball1Mass + ball2Mass);

        // These are the same
        double newVector1TangentMagnitude = vector1TangentMagnitude;
        double newVector2TangentMagnitude = vector2TangentMagnitude;

        Point* newVector1Normal = (*unitVector) * newVector1NormalMagnitude;
        Point* newVector1Tangent = (*tangentVector) * newVector1TangentMagnitude;
        Point* newVector2Normal = (*unitVector) * newVector2NormalMagnitude;
        Point* newVector2Tangent = (*tangentVector) * newVector2TangentMagnitude;

        Point* newVector1Velocity = (*newVector1Normal) + newVector1Tangent;
        Point* newVector2Velocity = (*newVector2Normal) + newVector2Tangent;

        ball1->setVelocity(newVector1Velocity);
        ball2->setVelocity(newVector2Velocity);
    }

    // If the balls are no longer touching, set colliding to false
    else if (magnitude > ballRad * 2) {
        colliding[ball1Idx][ball2Idx] = false;
        colliding[ball2Idx][ball1Idx] = false;
    }
}

void applyCollisions(Ball** balls, double ballRad, bool** colliding) {
    for (int i = 0; i < 16; i++)
        for (int j = i + 1; j < 16; j++)
            applyCollision(balls, i, j, ballRad, colliding);
}

bool ballsNotMoving(Ball** balls) {
    for (int i = 0; i < 16; i++)
        if (balls[i]->isMoving())
            return false;
    return true;
}

void drawBalls(Ball** balls) {
    int i = 0;
    balls[0]->draw(15, 15);
    for (int i = 1; i < 16; i++) {
        balls[i]->draw(15, 15);
    }
}


void camOnTopOfCue(float cueRotAng1, float cueRotAng2, Ball* whiteBall) {
    float x = 0;
    float y = 1;
    float z = 0;

    // Convert to radians
    cueRotAng1 = cueRotAng1 * M_PI / 180;
    cueRotAng2 = cueRotAng2 * M_PI / 180;

    // Create a vector rotatedX, rotatedY, rotatedZ with the direction of the cue
    // Vertical cue rotation
    float rotatedY = cos(cueRotAng2) * y - sin(cueRotAng2) * z;
    float rotatedZ = sin(cueRotAng2) * y + cos(cueRotAng2) * z;
    // Horizontal cue rotation
    float rotatedX = cos(cueRotAng1) * x - sin(cueRotAng1) * rotatedZ;
    rotatedZ = sin(cueRotAng1) * x + cos(cueRotAng1) * rotatedZ;

    // Make camera distance constant by dividing by the magnitude of the vector and multiplying by a factor
    float magnitude = sqrt(pow(rotatedX, 2) + pow(rotatedY, 2) + pow(rotatedZ, 2));
    rotatedX *= 7 / magnitude;
    rotatedY *= 7 / magnitude;
    rotatedZ *= 7 / magnitude;

    float camPosX = whiteBall->getPosX() - rotatedX;
    float camPosY = whiteBall->getPosY() + rotatedY + 2; // Add a constant so the camera is a bit above the cue
    float camPosZ = whiteBall->getPosZ() + rotatedZ;

    gluLookAt(camPosX, camPosY, camPosZ, whiteBall->getPosX(), whiteBall->getPosY(), whiteBall->getPosZ(), 0, 1, 0);
}

void drawWorld() {
    int roomLength = 20;
    int roomWidth = 15;
    int roomHeight = 20;
    float roomFloor = -2.49;

    glColor3ub(230, 230, 230);

    // Floor
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(roomLength, roomFloor, roomWidth);
    glVertex3f(roomLength, roomFloor, -roomWidth);
    glVertex3f(-roomLength, roomFloor, -roomWidth);
    glVertex3f(-roomLength, roomFloor, roomWidth);
    glEnd();
}

void initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    atexit(SDL_Quit);
    Uint32 flags = SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_OPENGL;

    if (SDL_SetVideoMode(1280, 720, 32, flags) == NULL) {
        cerr << "Failed to initialize view mode: " << SDL_GetError() << endl;
        exit(1);
    }

    if (SDL_EnableKeyRepeat(30, 10) < 0) {
        cerr << "Failed to initialize key-repeat mode: " << SDL_GetError() << endl;
        exit(1);
    }
}

// Returns a 16*16 boolean matrix with false in every entry
bool** getCollisionMatrix() {
    bool** colliding = new bool* [16];
    for (int i = 0; i < 16; i++)
        colliding[i] = new bool[16];
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
            colliding[i][j] = false;
    return colliding;
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

#undef main
int main(int argc, char* argv[]) {

    // Initialize viewMode 0 camera
    float camX = 0;
    float camY = 6;
    float camZ = -6;
    float camAngA = 0;
    float camAngB = -45;
    float camRad = -8.4852; // sqrt(y^2 + z^2)

    bool moveCam = false;
    bool moveCue = false;

    initializeSDL();

    glMatrixMode(GL_PROJECTION);
    float color = 0;
    glClearColor(color, color, color, 1);
    gluPerspective(45, 1280 / 720.f, 0.1, 100);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    // Size of table, balls and cue
    float tableLength = 7.9;
    float tableWidth = 3.5;
    double ballRad = tableLength / 50;
    double ballMass = 1;
    float ballSeparation = ballRad * 1.75;
    int cueLength = 2.3;

    // Initial cue rotation
    float cueRotAng1 = 90;
    float cueRotAng2 = -80;

    bool** colliding = getCollisionMatrix(); // if balls i and j are colliding => colliding[i][j] = true and colliding[j][i] = true
    Ball** balls = initializeBalls(ballRad, ballMass, ballSeparation);

    SDL_Event event;
    float strength = 4;
    int viewMode = 0;
    bool quit = false;
    bool pause = false;
    bool slowMotion = false;
    auto lastFrameTime = clock();
    bool showMenu = false;
    bool gameEnded = false;

    do {
        auto currentTime = clock();
        float frameTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = clock();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set camera position
        if (viewMode == 0)
            gluLookAt(camX, camY, -camZ, 0, 0, 0, 0, 1, 0);
        else if (viewMode == 1)
            gluLookAt(0, 7, 0, 0, 0, 0, 0, 0, -1);
        else
            camOnTopOfCue(cueRotAng1, cueRotAng2, balls[0]);

        setLighting();

        // Draw objects and apply physics
        drawWorld();
        drawBalls(balls);
        if (ballsNotMoving(balls))
            drawCue(10, 0.04, cueLength, balls[0], cueRotAng1, cueRotAng2, strength);
        if (!pause) {
            applyCollisions(balls, ballRad, colliding);
            if (slowMotion)
                moveBalls(balls, frameTime / 120, tableLength, tableWidth);
            else
                moveBalls(balls, frameTime / 40, tableLength, tableWidth);
        }

        // Process events
        int xm, ym;
        SDL_GetMouseState(&xm, &ym);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_MOUSEBUTTONDOWN:
                if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))
                    moveCam = true;
                if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))
                    moveCue = true;
                break;
            case SDL_MOUSEBUTTONUP:
                moveCam = false;
                if (moveCue) {
                    moveCue = false;
                    if (ballsNotMoving(balls) && !pause && strength > 4)
                        hitBall(balls[0], cueRotAng1, cueRotAng2, strength);
                    strength = 4;
                }
                break;

            case SDL_MOUSEMOTION:
                // Move cam
                if (moveCam && viewMode == 0) {
                    if (event.motion.yrel < 0 && camAngB < 80)
                        camAngB -= event.motion.yrel * 0.4;
                    else if (event.motion.yrel >= 0 && camAngB > -80)
                        camAngB -= event.motion.yrel * 0.4;

                    camAngA += event.motion.xrel * 0.4;
                    updateCam(camX, camY, camZ, camAngB, camAngA, camRad);
                }

                // Move cue
                if (moveCue && !pause && ballsNotMoving(balls)) {
                    cueRotAng1 += event.motion.xrel * 0.4;
                    strength += event.motion.yrel * 0.4;

                    if (strength > 20) strength = 20;
                    if (strength < 4) strength = 4;
                    while (cueRotAng1 >= 360) cueRotAng1 -= 360;
                    while (cueRotAng1 < 0) cueRotAng1 += 360;
                }

                break;
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYUP: {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_q:
                    quit = true;
                    break;
                case SDLK_p:
                    pause = !pause;
                    break;
                case SDLK_m:
                    slowMotion = !slowMotion;
                    break;
                default:
                    break;
                }
            }
           break;
            default:
                break;
            }
        }
        SDL_GL_SwapBuffers();

        // Force 30fps cap
        currentTime = clock();
        frameTime = (float)(currentTime - lastFrameTime);
        if (frameTime < 33.3)
            std::this_thread::sleep_for(std::chrono::milliseconds((int)(33.3 - frameTime)));


    } while (!quit);
    return 0;
}
