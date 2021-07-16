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
    bool drawOBBs = false;

    // FPS management
    clock_t lastFrameTime = clock();
    float timeSinceLastFrame = 0;

    // Collision detection algorithms
    BroadPhaseAlgorithm* broadPhaseAlgorithm = new NoBroadPhase();
    MidPhaseAlgorithm* midPhaseAlgorithm = new OBBMidPhase();
    NarrowPhaseAlgorithm* narrowPhaseAlgorithm = new NarrowPhaseAlgorithm();

    // Scene
    string sceneName = "scene.xml";
    vector<Object*> objectsVector = ObjectLoader(sceneName, config.numLatLongs).getObjects();
    Object** objects = &objectsVector[0];
    int numObjects = objectsVector.size();

    // Logging
    chrono::system_clock::time_point frameStart, broadEnd, midEnd, narrowEnd, responseEnd, moveEnd;
    std::ofstream outputCSV;
    outputCSV.open("output.csv");
    outputCSV << "BPCDCount,BPCDTime,MPCDCount,MPCDTime,NPCDCount,NPCDTime,CRTime,MTime,TTime\n";

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
            drawObjects(objects, numObjects, drawOBBs);
        }

        // Apply physics and movement
        if (!pause) {
            auto frameStart = std::chrono::system_clock::now();
            map<string, pair<Object*, Object*>> broadPhaseCollisions = broadPhaseAlgorithm->getCollisions(objects, numObjects);
            broadEnd = std::chrono::system_clock::now();
            map<string, pair<Object*, Object*>> midPhaseCollisions = midPhaseAlgorithm->getCollisions(broadPhaseCollisions);
            midEnd = std::chrono::system_clock::now();
            map<string, Collision> collisions = narrowPhaseAlgorithm->getCollisions(midPhaseCollisions);
            narrowEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::collisionResponse(collisions);
            responseEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::moveObjects(objects, numObjects, 1.0 / config.fps, slowMotion);
            moveEnd = std::chrono::system_clock::now();

            // Log times
            outputCSV << broadPhaseCollisions.size();
            outputCSV << ",";
            outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(broadEnd - frameStart).count() / 1000;
            outputCSV << ",";
            outputCSV << midPhaseCollisions.size();
            outputCSV << ",";
            outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(midEnd - broadEnd).count() / 1000;
            outputCSV << ",";
            outputCSV << collisions.size();
            outputCSV << ",";
            outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(narrowEnd - midEnd).count() / 1000;
            outputCSV << ",";
            outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(responseEnd - narrowEnd).count() / 1000;
            outputCSV << ",";
            outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(moveEnd - responseEnd).count() / 1000;
            outputCSV << ",";
            outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(narrowEnd - frameStart).count() / 1000;
            outputCSV << ",";
            outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(moveEnd - frameStart).count() / 1000;
            outputCSV << "\n";
        }

        // Process events
        checkForInput(slowMotion, pause, quit, draw, drawOBBs, camera, freeCamera, centeredCamera);

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame, config.fps);

        SDL_GL_SwapWindow(window);
    }

    outputCSV.close();
    return 0;
}
