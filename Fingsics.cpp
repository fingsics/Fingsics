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
#include <filesystem>

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

void log(std::ofstream& outputCSV, int numBroadPhaseCollisions, int numMidPhaseCollisions, int numCollisions, chrono::system_clock::time_point frameStart, chrono::system_clock::time_point broadEnd, chrono::system_clock::time_point midEnd, chrono::system_clock::time_point narrowEnd, chrono::system_clock::time_point responseEnd, chrono::system_clock::time_point moveEnd) {
    outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(broadEnd - frameStart).count() / 1000.0;
    outputCSV << ",";
    outputCSV << numBroadPhaseCollisions;
    outputCSV << ",";
    outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(midEnd - broadEnd).count() / 1000.0;
    outputCSV << ",";
    outputCSV << numMidPhaseCollisions;
    outputCSV << ",";
    outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(narrowEnd - midEnd).count() / 1000.0;
    outputCSV << ",";
    outputCSV << numCollisions;
    outputCSV << ",";
    outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(responseEnd - narrowEnd).count() / 1000.0;
    outputCSV << ",";
    outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(moveEnd - responseEnd).count() / 1000.0;
    outputCSV << ",";
    outputCSV << (float)chrono::duration_cast<std::chrono::microseconds>(moveEnd - frameStart).count() / 1000.0;
    outputCSV << "\n";
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
    bool drawOBBs = true;
    bool drawAABBs = false;

    // FPS management
    clock_t lastFrameTime = clock();
    float timeSinceLastFrame = 0;

    // Scene
    vector<Object*> objectsVector = ObjectLoader(config.sceneName + ".xml", config.numLatLongs).getObjects();
    Object** objects = &objectsVector[0];
    int numObjects = objectsVector.size();

    // Collision detection algorithms
    BroadPhaseAlgorithm* broadPhaseAlgorithm = new SweepAndPruneBroadPhase(objects, numObjects);
    //BroadPhaseAlgorithm* broadPhaseAlgorithm = new BruteForceBroadPhase();
    MidPhaseAlgorithm* midPhaseAlgorithm = new NoMidPhase();
    NarrowPhaseAlgorithm* narrowPhaseAlgorithm = new NarrowPhaseAlgorithm();

    // Logging
    chrono::system_clock::time_point frameStart, broadEnd, midEnd, narrowEnd, responseEnd, moveEnd;
    std::ofstream outputCSV;

    if (config.log) {
        outputCSV.open("output\\" + config.logOutputFile);
        outputCSV << "BPCDTime,MPCDTests,MPCDTime,NPCDTests,NPCDTime,Collisions,CRTime,MoveTime,TotalTime\n";
    }

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
            drawObjects(objects, numObjects, drawOBBs, drawAABBs);
        }

        // Apply physics and movement
        if (!pause) {
            if (config.log) frameStart = std::chrono::system_clock::now();
            map<string, pair<Object*, Object*>> broadPhaseCollisions = broadPhaseAlgorithm->getCollisions(objects, numObjects);
            if (config.log) broadEnd = std::chrono::system_clock::now();
            map<string, pair<Object*, Object*>> midPhaseCollisions = midPhaseAlgorithm->getCollisions(broadPhaseCollisions);
            if (config.log) midEnd = std::chrono::system_clock::now();
            map<string, Collision> collisions = narrowPhaseAlgorithm->getCollisions(midPhaseCollisions);
            if (config.log) narrowEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::collisionResponse(collisions);
            if (config.log) responseEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::moveObjects(objects, numObjects, 1.0 / config.fps, slowMotion);
            if (config.log) {
                moveEnd = std::chrono::system_clock::now();
                log(outputCSV, broadPhaseCollisions.size(), midPhaseCollisions.size(), collisions.size(), frameStart, broadEnd, midEnd, narrowEnd, responseEnd, moveEnd);
            }
        }

        // Process events
        checkForInput(slowMotion, pause, quit, draw, drawOBBs, camera, freeCamera, centeredCamera);

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame, config.fps);

        SDL_GL_SwapWindow(window);
    }

    if (config.log) outputCSV.close();
    return 0;
}
