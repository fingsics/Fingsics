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
#include <map>
#include <glm/glm.hpp>
#include <glut.h>

#define _USE_MATH_DEFINES
#define FPS 30

using namespace std;

// TODO: load from XML
Ball** initializeScene(double ballRad, double ballMass) {
    Ball** balls = new Ball * [16];
    Color* ballColor = new Color(200, 20, 20);
    Color* ballColor2 = new Color(200, 200, 200);
    float height = ballRad;
    float whiteBallDistance = 1.5;
    float ballSeparation = ballRad * 1.75;

    // White ball
    balls[0] = new Ball("0", -2, height, 0, ballRad, ballMass, ballColor2);
    // First line
    balls[1] = new Ball("1", whiteBallDistance, height, 0, ballRad, ballMass, ballColor);
    // Second line
    balls[2] = new Ball("2", whiteBallDistance + ballSeparation, height, ballRad, ballRad, ballMass, ballColor);
    balls[3] = new Ball("3", whiteBallDistance + ballSeparation, height, -ballRad, ballRad, ballMass, ballColor);
    // Third line
    balls[4] = new Ball("4", whiteBallDistance + ballSeparation * 2, height, 0, ballRad, ballMass, ballColor);
    balls[5] = new Ball("5", whiteBallDistance + ballSeparation * 2, height, ballRad * 2, ballRad, ballMass, ballColor);
    balls[6] = new Ball("6", whiteBallDistance + ballSeparation * 2, height, -ballRad * 2, ballRad, ballMass, ballColor);
    // Forth line
    balls[7] = new Ball("7", whiteBallDistance + ballSeparation * 3, height, ballRad, ballRad, ballMass, ballColor);
    balls[8] = new Ball("8", whiteBallDistance + ballSeparation * 3, height, -ballRad, ballRad, ballMass, ballColor);
    balls[9] = new Ball("9", whiteBallDistance + ballSeparation * 3, height, ballRad + ballRad * 2, ballRad, ballMass, ballColor);
    balls[10] = new Ball("10", whiteBallDistance + ballSeparation * 3, height, -ballRad - ballRad * 2, ballRad, ballMass, ballColor);
    // Fifth line
    balls[11] = new Ball("11", whiteBallDistance + ballSeparation * 4, height, 0, ballRad, ballMass, ballColor);
    balls[12] = new Ball("12", whiteBallDistance + ballSeparation * 4, height, ballRad * 2, ballRad, ballMass, ballColor);
    balls[13] = new Ball("13", whiteBallDistance + ballSeparation * 4, height, ballRad * 4, ballRad, ballMass, ballColor);
    balls[14] = new Ball("14", whiteBallDistance + ballSeparation * 4, height, -ballRad * 2, ballRad, ballMass, ballColor);
    balls[15] = new Ball("15", whiteBallDistance + ballSeparation * 4, height, -ballRad * 4, ballRad, ballMass, ballColor);

    // Set white ball initial velocity
    balls[0]->setVelocity(new Point(25, 0, 0));

    return balls;
}

void moveObjects(Ball** balls, float frames, bool slowMotion) {
    float time = slowMotion ? frames / 3 : frames;
    for (int i = 0; i < 16; i++) balls[i]->updatePosAndVel(time, balls);
}

void applyCollisions(map<string, pair<Ball*, Ball*>> oldCollisions, map<string, pair<Ball*, Ball*>> collisionMap) {
    for (auto const& mapEntry : collisionMap) {
        if (oldCollisions.find(mapEntry.first) != oldCollisions.end()) continue;
        pair<Ball*, Ball*> collisionPair = mapEntry.second;
        Ball* ball1 = collisionPair.first;
        Ball* ball2 = collisionPair.second;
        double ball1Mass = ball1->getMass();
        double ball2Mass = ball2->getMass();
        Point* ball1Vel = ball1->getVel();
        Point* ball2Vel = ball2->getVel();
        Point* ball1Pos = ball1->getPos();
        Point* ball2Pos = ball2->getPos();

        Point* normalVector = (*ball1Pos) - ball2Pos;
        double distance = normalVector->magnitude();

        Point* unitVector = (*normalVector) / distance;
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
}

string getObjectPairId(pair<Ball*, Ball*> objectPair) {
    string firstKey = objectPair.first->getId() < objectPair.second->getId() ? objectPair.first->getId() : objectPair.second->getId();
    string secondKey = objectPair.first->getId() < objectPair.second->getId() ? objectPair.second->getId() : objectPair.first->getId();
    return firstKey + "-" + secondKey;
}

// TODO: implement BPCD algorithm as a class that inherits from an interface
map<string, pair<Ball*, Ball*>> getCollisions(Ball** balls) {
    map<string, pair<Ball*, Ball*>> collisionMap;
    for (int i = 0; i < 16; i++)
        for (int j = i + 1; j < 16; j++) {
            Ball* ball1 = balls[i];
            Ball* ball2 = balls[j];
            Point* ball1Pos = ball1->getPos();
            Point* ball2Pos = ball2->getPos();

            Point* normalVector = (*ball1Pos) - ball2Pos;
            double distance = normalVector->magnitude();

            if (distance < ball1->getRad() + ball2->getRad()) {
                pair<Ball*, Ball*> objectPair = make_pair(ball1, ball2);
                string objectPairId = getObjectPairId(objectPair);
                if (collisionMap.find(objectPairId) == collisionMap.end()) {
                    collisionMap.insert(pair<string, pair<Ball*, Ball*>>(objectPairId, objectPair));
                }
            }
        }
    return collisionMap;
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

void checkForInput(bool &slowMotion, bool &pause, bool &quit) {
    SDL_Event event;
    int xm, ym;
    SDL_GetMouseState(&xm, &ym);
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
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
        default:
            break;
        }
    }
}

void manageFrameTime(clock_t &lastFrameTime, float &timeSinceLastFrame) {
    float minFrameTime = 1000 / FPS;
    timeSinceLastFrame = (float)(clock() - lastFrameTime);
    if (timeSinceLastFrame < minFrameTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(minFrameTime - timeSinceLastFrame)));
        timeSinceLastFrame = minFrameTime;
    }
    
    lastFrameTime = clock();
}

#undef main
int main(int argc, char* argv[]) {
    initializeSDL();

    glMatrixMode(GL_PROJECTION);
    glClearColor(0, 0, 0, 1);
    gluPerspective(45, 1280 / 720.f, 0.1, 100);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    double ballRad = 0.2;
    double ballMass = 1;

    Ball** balls = initializeScene(ballRad, ballMass);
    bool quit = false;
    bool pause = false;
    bool slowMotion = false;
    bool showMenu = false;

    map<string, pair<Ball*, Ball*>> oldCollisions;

    clock_t lastFrameTime = clock();
    float timeSinceLastFrame = 0;

    while (!quit) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set camera position
        gluLookAt(0, 7, 0, 0, 0, 0, 0, 0, -1);

        // Set light
        setLighting();

        // Draw objects
        drawFloor();
        drawBalls(balls);

        // Apply physics and movement
        if (!pause) {
            map<string, pair<Ball*, Ball*>> collisions = getCollisions(balls);
            applyCollisions(oldCollisions, collisions);
            oldCollisions = collisions;
            moveObjects(balls, timeSinceLastFrame / 40, slowMotion);
        }

        // Process events
        checkForInput(slowMotion, pause, quit);

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame);

        SDL_GL_SwapBuffers();
    }
    return 0;
}
