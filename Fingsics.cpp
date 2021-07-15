// Fingsics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "include/SDLHelpers.h";
#include "include/OpenGlHelpers.h";
#include "include/ObjectLoader.h"
#include "include/ConfigLoader.h"
#include "include/CenteredCamera.h"
#include "include/FreeCamera.h"
#include "include/BroadPhaseAlgorithms.h"
#include "include/MidPhaseAlgorithms.h"
#include "include/NarrowPhaseAlgorithm.h"
#include "include/CollisionResponseAlgorithm.h"
#include <iostream>
#include <thread>
#include <map>

#define _USE_MATH_DEFINES

using namespace std;

void manageFrameTime(clock_t &lastFrameTime, float &secondsSinceLastFrame, int fps) {
    double minFrameTime = 1.0 / fps;
    secondsSinceLastFrame = (double)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    if (secondsSinceLastFrame < minFrameTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)((minFrameTime - secondsSinceLastFrame) * 1000)));
        secondsSinceLastFrame = (double)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    }
    lastFrameTime = clock();
}

int main(int argc, char* argv[]) {
    SDL_Window* window = initializeSDL();

    Config config = ConfigLoader().getConfig();

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
    vector<Object*> objectsVector = ObjectLoader(sceneName, config.numLatLongs).getObjects();
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
            CollisionResponseAlgorithm::collisionResponse(collisions);
            CollisionResponseAlgorithm::moveObjects(objects, numObjects, 1.0 / config.fps, slowMotion);
        }

        // Process events
        checkForInput(slowMotion, pause, quit, draw, camera, freeCamera, centeredCamera);

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame, config.fps);

        SDL_GL_SwapWindow(window);
    }
    return 0;
}
