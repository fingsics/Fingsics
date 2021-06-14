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
#define FPS 30

using namespace std;

Ball** initializeBalls(double ballRad, double ballMass, float ballSeparation) {
    Ball** balls = new Ball * [16];
    Color* ballColor = new Color(200, 20, 20);
    Color* ballColor2 = new Color(200, 200, 200);
    float height = ballRad;
    float whiteBallDistance = 1.5;

    // White ball
    balls[0] = new Ball(-2, height, 0, ballRad, ballMass, ballColor2);
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

void moveBalls(Ball** balls, float frames, bool slowMotion) {
    float time = slowMotion ? frames / 3 : frames;
    for (int i = 0; i < 16; i++) balls[i]->updatePosAndVel(time, balls);
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
    balls[0]->draw();
    for (int i = 1; i < 16; i++) {
        balls[i]->draw();
    }
}

void drawFloor() {
    int roomLength = 20;
    int roomWidth = 15;
    int roomHeight = 20;
    float roomFloor = 0;

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
    // if balls i and j are colliding => colliding[i][j] = true and colliding[j][i] = true
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

void hitBall(Ball* whiteBall) {
    float strengthFactor = 15;
    float x = 1;
    float y = 0;
    whiteBall->setVelocity(new Point(x * strengthFactor, 0, y * strengthFactor));
}

#undef main
int main(int argc, char* argv[]) {
    initializeSDL();

    glMatrixMode(GL_PROJECTION);
    float color = 0;
    glClearColor(color, color, color, 1);
    gluPerspective(45, 1280 / 720.f, 0.1, 100);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    // Size of table, balls and cue
    double ballRad = 0.2;
    double ballMass = 1;
    float ballSeparation = ballRad * 1.75;

    bool** colliding = getCollisionMatrix();
    Ball** balls = initializeBalls(ballRad, ballMass, ballSeparation);

    SDL_Event event;
    bool quit = false;
    bool pause = false;
    bool slowMotion = false;
    auto lastFrameTime = clock();
    bool showMenu = false;

    do {
        auto currentTime = clock();
        float frameTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = clock();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set camera position
        gluLookAt(0, 7, 0, 0, 0, 0, 0, 0, -1);

        setLighting();

        // Draw objects and apply physics
        drawFloor();
        drawBalls(balls);
        if (!pause) {
            applyCollisions(balls, ballRad, colliding);
            moveBalls(balls, frameTime / 40, slowMotion);
        }

        // Process events
        int xm, ym;
        SDL_GetMouseState(&xm, &ym);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONUP:
                hitBall(balls[0]);
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

        // Force FPS cap
        float minFrameTime = 1000 / FPS;
        currentTime = clock();
        frameTime = (float)(currentTime - lastFrameTime);
        if (frameTime < minFrameTime)
            std::this_thread::sleep_for(std::chrono::milliseconds((int)(minFrameTime - frameTime)));


    } while (!quit);
    return 0;
}
