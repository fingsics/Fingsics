// Fingsics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "include/SDLHelpers.h";
#include "include/OpenGlHelpers.h";
#include "include/ObjectLoader.h"
#include "include/CenteredCamera.h"
#include "include/FreeCamera.h"
#include "include/BroadPhaseAlgorithms.h"
#include "include/MidPhaseAlgorithms.h"
#include "include/NarrowPhaseAlgorithm.h"
#include "include/CollisionResponseAlgorithms.h"
#include <iostream>
#include <thread>
#include <map>

#define _USE_MATH_DEFINES
#define FPS 60

using namespace std;

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
    Camera* camera = centeredCamera;

    // Program options
    bool quit = false;
    bool pause = true;
    bool draw = true;
    bool slowMotion = false;
    bool showMenu = false;

    // FPS management
    clock_t lastFrameTime = clock();
    float timeSinceLastFrame = 0;

    // Collision detection algorithms
    BroadPhaseAlgorithm* broadPhaseAlgorithm = new NoBroadPhase();
    MidPhaseAlgorithm* midPhaseAlgorithm = new NoMidPhase();
    NarrowPhaseAlgorithm* narrowPhaseAlgorithm = new NarrowPhaseAlgorithm();

    // Scene
    string sceneName = "bouncy-things.xml";
    ObjectLoader scene = ObjectLoader(sceneName);
    vector<Object*> objectsVector = scene.getObjects();
    Object** objects = &objectsVector[0];
    int numObjects = objectsVector.size();

    initializeOpenGL();

    while (!quit) {
        setupFrame();

        // Set camera position
        camera->lookAt();

        // Set light
        setLighting();

        // Draw objects
        if (draw) {
            drawAxis();
            drawObjects(objects, numObjects);
        }

        // Apply physics and movement
        if (!pause) {
            map<string, pair<Object*, Object*>> broadPhaseCollisions = broadPhaseAlgorithm->getCollisions(objects, numObjects);
            map<string, pair<Object*, Object*>> midPhaseCollisions = midPhaseAlgorithm->getCollisions(broadPhaseCollisions);
            map<string, Collision> collisions = narrowPhaseAlgorithm->getCollisions(midPhaseCollisions);
            collisionResponse(collisions);
            moveObjects(objects, numObjects, 1.0 / FPS, slowMotion);
        }

        // Process events
        checkForInput(slowMotion, pause, quit, draw, camera, freeCamera, centeredCamera);

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame);

        SDL_GL_SwapWindow(window);
    }
    return 0;
}
