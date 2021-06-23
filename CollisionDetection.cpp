// CollisionDetection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "SDL.h"
#include "SDL_opengl.h"
#include "include/Scene.h"
#include "include/CenteredCamera.h"
#include "include/FreeCamera.h"
#include "include/BroadPhaseAlgorithm.h"
#include "include/BruteForceBPA.h"
#include "freeglut.h"
#include <iostream>
#include <thread>
#include <string>
#include <map>

#define _USE_MATH_DEFINES
#define FPS 60

using namespace std;

void moveObjects(Object** objects, int numObjects, float frames, bool slowMotion, Room room) {
    float time = slowMotion ? frames / 3 : frames;
    for (int i = 0; i < numObjects; i++) objects[i]->updatePosAndVel(time, room);
}

void applyCollisions(map<string, pair<Object*, Object*>> oldCollisions, map<string, pair<Object*, Object*>> collisionMap) {
    for (auto const& mapEntry : collisionMap) {
        if (oldCollisions.find(mapEntry.first) != oldCollisions.end()) continue;
        pair<Object*, Object*> collisionPair = mapEntry.second;
        Object* object1 = collisionPair.first;
        Object* object2 = collisionPair.second;
        double object1Mass = object1->getMass();
        double object2Mass = object2->getMass();
        Point object1Vel = object1->getVel();
        Point object2Vel = object2->getVel();
        Point object1Pos = object1->getPos();
        Point object2Pos = object2->getPos();

        Point normalVector = object1Pos - object2Pos;
        double distance = normalVector.magnitude();

        Point unitVector = normalVector / distance;
        Point tangentVector = Point(-unitVector.getZ(), -unitVector.getY(), unitVector.getX());

        double vector1NormalMagnitude = unitVector.dotProduct(object1Vel);
        double vector1TangentMagnitude = tangentVector.dotProduct(object1Vel);
        double vector2NormalMagnitude = unitVector.dotProduct(object2Vel);
        double vector2TangentMagnitude = tangentVector.dotProduct(object2Vel);

        // Because of conservation of kinetic energy
        double newVector1NormalMagnitude = (vector1NormalMagnitude * (object1Mass - object2Mass) + 2 * object2Mass * vector2NormalMagnitude) / (object1Mass + object2Mass);
        double newVector2NormalMagnitude = (vector2NormalMagnitude * (object2Mass - object1Mass) + 2 * object1Mass * vector1NormalMagnitude) / (object1Mass + object2Mass);

        // These are the same
        double newVector1TangentMagnitude = vector1TangentMagnitude;
        double newVector2TangentMagnitude = vector2TangentMagnitude;

        Point newVector1Normal = unitVector * newVector1NormalMagnitude;
        Point newVector1Tangent = tangentVector * newVector1TangentMagnitude;
        Point newVector2Normal = unitVector * newVector2NormalMagnitude;
        Point newVector2Tangent = tangentVector * newVector2TangentMagnitude;

        Point newVector1Velocity = (newVector1Normal + newVector1Tangent) * object1->getElasticity();
        Point newVector2Velocity = (newVector2Normal + newVector2Tangent) * object2->getElasticity();

        object1->setVel(newVector1Velocity);
        object2->setVel(newVector2Velocity);
    }
}

void drawObjects(Object** objects, int numObjects) {
    int i = 0;
    for (int i = 0; i < numObjects; i++) {
        objects[i]->draw();
    }
}

SDL_Window* initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    atexit(SDL_Quit);

    SDL_Window* window = SDL_CreateWindow("Fingsycs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        cerr << "Failed to initialize view mode: " << SDL_GetError() << endl;
        exit(1);
    }
    SDL_GL_CreateContext(window);

    return window;
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

void checkForInput(bool &slowMotion, bool &pause, bool &quit, bool& draw, Camera* &camera, Camera* freeCamera, Camera* centeredCamera) {
    SDL_Event event;
    int xm, ym;
    SDL_GetMouseState(&xm, &ym);
    while (SDL_PollEvent(&event)) {
        camera->eventUpdate(event);
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
            case SDLK_f:
                draw = !draw;
                break;
            case SDLK_c:
                if (camera == centeredCamera) camera = freeCamera;
                else camera = centeredCamera;
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

void manageFrameTime(clock_t &lastFrameTime, float &secondsSinceLastFrame) {
    double minFrameTime = 1.0 / FPS;
    secondsSinceLastFrame = (double)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    if (secondsSinceLastFrame < minFrameTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)((minFrameTime - secondsSinceLastFrame) * 1000)));
        secondsSinceLastFrame = (double)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    }
    lastFrameTime = clock();
}

int main(int argc, char* argv[]) {
    SDL_Window* window = initializeSDL();

    // Camera
    Camera* centeredCamera = new CenteredCamera();
    Camera* freeCamera = new FreeCamera();
    Camera* camera = freeCamera;

    // Program options
    bool quit = false;
    bool pause = true;
    bool draw = true;
    bool slowMotion = false;
    bool showMenu = false;

    // FPS management
    clock_t lastFrameTime = clock();
    float timeSinceLastFrame = 0;

    // Collision detection algorithm
    BroadPhaseAlgorithm* broadPhaseAlgorithm = new BruteForceBPA();
    map<string, pair<Object*, Object*>> oldCollisions;

    // Scene
    string sceneName = "scene.xml";
    Scene scene = Scene(sceneName);
    Room room = scene.getRoom();
    vector<Object*> objectsVector = scene.getObjects();
    Object** objects = &objectsVector[0];
    int numObjects = objectsVector.size();

    glMatrixMode(GL_PROJECTION);
    glClearColor(0, 0, 0, 1);
    gluPerspective(45, 1280 / 720.f, 0.1, 100);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    while (!quit) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set camera position
        camera->lookAt();

        // Set light
        setLighting();

        // Draw objects
        if (draw) {
            room.draw();
            drawObjects(objects, numObjects);
        }

        // Apply physics and movement
        if (!pause) {
            map<string, pair<Object*, Object*>> collisions = broadPhaseAlgorithm->getCollisions(objects, numObjects);
            applyCollisions(oldCollisions, collisions);
            oldCollisions = collisions;
            moveObjects(objects, numObjects, timeSinceLastFrame, slowMotion, room);
        }

        // Process events
        checkForInput(slowMotion, pause, quit, draw, camera, freeCamera, centeredCamera);

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame);

        SDL_GL_SwapWindow(window);
    }
    return 0;
}
