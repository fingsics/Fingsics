// CollisionDetection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "include/Color.h"
#include "include/Point.h"
#include "include/Ball.h"
#include "include/Object.h"
#include "include/BroadPhaseAlgorithm.h"
#include "include/BruteForceBPA.h"
#include <thread>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glut.h>

#define _USE_MATH_DEFINES
#define FPS 30

using namespace std;

// TODO: load from XML
Object** initializeScene(double ballRad, double ballMass) {
    Object** objects = new Object * [16];
    Color* ballColor = new Color(200, 20, 20);
    Color* ballColor2 = new Color(200, 200, 200);
    float height = ballRad;
    float whiteBallDistance = 1.5;
    float ballSeparation = ballRad * 1.75;

    // White ball
    objects[0] = new Ball("0", -2, height, 0, ballRad, ballMass, ballColor2);
    // First line
    objects[1] = new Ball("1", whiteBallDistance, height, 0, ballRad, ballMass, ballColor);
    // Second line
    objects[2] = new Ball("2", whiteBallDistance + ballSeparation, height, ballRad, ballRad, ballMass, ballColor);
    objects[3] = new Ball("3", whiteBallDistance + ballSeparation, height, -ballRad, ballRad, ballMass, ballColor);
    // Third line
    objects[4] = new Ball("4", whiteBallDistance + ballSeparation * 2, height, 0, ballRad, ballMass, ballColor);
    objects[5] = new Ball("5", whiteBallDistance + ballSeparation * 2, height, ballRad * 2, ballRad, ballMass, ballColor);
    objects[6] = new Ball("6", whiteBallDistance + ballSeparation * 2, height, -ballRad * 2, ballRad, ballMass, ballColor);
    // Forth line
    objects[7] = new Ball("7", whiteBallDistance + ballSeparation * 3, height, ballRad, ballRad, ballMass, ballColor);
    objects[8] = new Ball("8", whiteBallDistance + ballSeparation * 3, height, -ballRad, ballRad, ballMass, ballColor);
    objects[9] = new Ball("9", whiteBallDistance + ballSeparation * 3, height, ballRad + ballRad * 2, ballRad, ballMass, ballColor);
    objects[10] = new Ball("10", whiteBallDistance + ballSeparation * 3, height, -ballRad - ballRad * 2, ballRad, ballMass, ballColor);
    // Fifth line
    objects[11] = new Ball("11", whiteBallDistance + ballSeparation * 4, height, 0, ballRad, ballMass, ballColor);
    objects[12] = new Ball("12", whiteBallDistance + ballSeparation * 4, height, ballRad * 2, ballRad, ballMass, ballColor);
    objects[13] = new Ball("13", whiteBallDistance + ballSeparation * 4, height, ballRad * 4, ballRad, ballMass, ballColor);
    objects[14] = new Ball("14", whiteBallDistance + ballSeparation * 4, height, -ballRad * 2, ballRad, ballMass, ballColor);
    objects[15] = new Ball("15", whiteBallDistance + ballSeparation * 4, height, -ballRad * 4, ballRad, ballMass, ballColor);

    // Set white ball initial velocity
    objects[0]->setVelocity(new Point(25, 0, 0));

    return objects;
}

void moveObjects(Object** objects, float frames, bool slowMotion) {
    float time = slowMotion ? frames / 3 : frames;
    for (int i = 0; i < 16; i++) objects[i]->updatePosAndVel(time);
}

void applyCollisions(map<string, pair<Object*, Object*>> oldCollisions, map<string, pair<Object*, Object*>> collisionMap) {
    for (auto const& mapEntry : collisionMap) {
        if (oldCollisions.find(mapEntry.first) != oldCollisions.end()) continue;
        pair<Object*, Object*> collisionPair = mapEntry.second;
        Object* object1 = collisionPair.first;
        Object* object2 = collisionPair.second;
        double object1Mass = object1->getMass();
        double object2Mass = object2->getMass();
        Point* object1Vel = object1->getVel();
        Point* object2Vel = object2->getVel();
        Point* object1Pos = object1->getPos();
        Point* object2Pos = object2->getPos();

        Point* normalVector = (*object1Pos) - object2Pos;
        double distance = normalVector->magnitude();

        Point* unitVector = (*normalVector) / distance;
        Point* tangentVector = new Point(-unitVector->getZ(), -unitVector->getY(), unitVector->getX());

        double vector1NormalMagnitude = unitVector->dotProduct(object1Vel);
        double vector1TangentMagnitude = tangentVector->dotProduct(object1Vel);
        double vector2NormalMagnitude = unitVector->dotProduct(object2Vel);
        double vector2TangentMagnitude = tangentVector->dotProduct(object2Vel);

        // Because of conservation of kinetic energy
        double newVector1NormalMagnitude = (vector1NormalMagnitude * (object1Mass - object2Mass) + 2 * object2Mass * vector2NormalMagnitude) / (object1Mass + object2Mass);
        double newVector2NormalMagnitude = (vector2NormalMagnitude * (object2Mass - object1Mass) + 2 * object1Mass * vector1NormalMagnitude) / (object1Mass + object2Mass);

        // These are the same
        double newVector1TangentMagnitude = vector1TangentMagnitude;
        double newVector2TangentMagnitude = vector2TangentMagnitude;

        Point* newVector1Normal = (*unitVector) * newVector1NormalMagnitude;
        Point* newVector1Tangent = (*tangentVector) * newVector1TangentMagnitude;
        Point* newVector2Normal = (*unitVector) * newVector2NormalMagnitude;
        Point* newVector2Tangent = (*tangentVector) * newVector2TangentMagnitude;

        Point* newVector1Velocity = (*newVector1Normal) + newVector1Tangent;
        Point* newVector2Velocity = (*newVector2Normal) + newVector2Tangent;

        object1->setVelocity(newVector1Velocity);
        object2->setVelocity(newVector2Velocity);
    }
}

bool objectsNotMoving(Object** objects) {
    for (int i = 0; i < 16; i++)
        if (objects[i]->isMoving())
            return false;
    return true;
}

void drawObjects(Object** objects) {
    int i = 0;
    for (int i = 0; i < 16; i++) {
        objects[i]->draw();
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

    Object** objects = initializeScene(ballRad, ballMass);
    bool quit = false;
    bool pause = false;
    bool slowMotion = false;
    bool showMenu = false;

    BroadPhaseAlgorithm* broadPhaseAlgorithm = new BruteForceBPA();
    map<string, pair<Object*, Object*>> oldCollisions;

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
        drawObjects(objects);

        // Apply physics and movement
        if (!pause) {
            map<string, pair<Object*, Object*>> collisions = broadPhaseAlgorithm->getCollisions(objects);
            applyCollisions(oldCollisions, collisions);
            oldCollisions = collisions;
            moveObjects(objects, timeSinceLastFrame / 40, slowMotion);
        }

        // Process events
        checkForInput(slowMotion, pause, quit);

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame);

        SDL_GL_SwapBuffers();
    }
    return 0;
}
