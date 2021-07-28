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

void manageFrameTime(clock_t &lastFrameTime, float &secondsSinceLastFrame, int fps, bool shouldSleep) {
    double minFrameTime = 1.0 / fps;
    secondsSinceLastFrame = (double)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    if (secondsSinceLastFrame < minFrameTime && shouldSleep) {
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

int runSimulation(Config config, int stopFrame, SDL_Window* window) {
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
    bool drawAABBs = false;

    // FPS management
    clock_t lastFrameTime = clock();
    float timeSinceLastFrame = 0;
    int frame = 0;

    // Scene
    vector<Object*> objectsVector = ObjectLoader(config.sceneName + ".xml", config.numLatLongs).getObjects();
    Object** objects = &objectsVector[0];
    int numObjects = objectsVector.size();

    // Collision detection algorithms
    NarrowPhaseAlgorithm* narrowPhaseAlgorithm = new NarrowPhaseAlgorithm();
    MidPhaseAlgorithm* midPhaseAlgorithm;
    if (config.useMidPhase) midPhaseAlgorithm = new OBBMidPhase();
    else midPhaseAlgorithm = new NoMidPhase();

    BroadPhaseAlgorithm* broadPhaseAlgorithm;
    switch (config.bpAlgorithm) {
    case BPAlgorithmChoice::none:
        broadPhaseAlgorithm = new NoBroadPhase();
        break;
    case BPAlgorithmChoice::bruteForce:
        broadPhaseAlgorithm = new BruteForceBroadPhase();
        break;
    default:
        broadPhaseAlgorithm = new SweepAndPruneBroadPhase(objects, numObjects);
        break;
    }

    // Logging
    chrono::system_clock::time_point frameStart, broadEnd, midEnd, narrowEnd, responseEnd, moveEnd;
    std::ofstream outputCSV;

    if (config.log || config.runInTestMode) {
        string folder = config.runInTestMode ? "testing\\results\\" : "output\\";
        outputCSV.open(folder + config.logOutputFile);
        outputCSV << "BPCDTime,MPCDTests,MPCDTime,NPCDTests,NPCDTime,Collisions,CRTime,MoveTime,TotalTime\n";
    }

    initializeOpenGL();

    // Collision collections
    map<string, pair<Object*, Object*>> broadPhaseCollisions, midPhaseCollisions;
    map<string, Collision> collisions;

    while (!quit && (stopFrame == -1 || frame < stopFrame)) {
        if (!config.runInTestMode) {
            setupFrame();
            camera->lookAt(); // Set camera position
            setLighting(); // Set light
        }

        // Draw objects
        if (draw && !config.runInTestMode) {
            drawAxis();
            drawObjects(objects, numObjects, drawOBBs, drawAABBs);
        }

        // Apply physics and movement
        if (!pause || config.runInTestMode) {
            if (config.log || config.runInTestMode) frameStart = std::chrono::system_clock::now();
            broadPhaseCollisions = broadPhaseAlgorithm->getCollisions(objects, numObjects);
            if (config.log || config.runInTestMode) broadEnd = std::chrono::system_clock::now();
            midPhaseCollisions = midPhaseAlgorithm->getCollisions(broadPhaseCollisions);
            if (config.log || config.runInTestMode) midEnd = std::chrono::system_clock::now();
            collisions = narrowPhaseAlgorithm->getCollisions(midPhaseCollisions);
            if (config.log || config.runInTestMode) narrowEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::collisionResponse(collisions);
            if (config.log || config.runInTestMode) responseEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::moveObjects(objects, numObjects, 1.0 / config.fps, slowMotion);
            if (config.log || config.runInTestMode) {
                moveEnd = std::chrono::system_clock::now();
                log(outputCSV, broadPhaseCollisions.size(), midPhaseCollisions.size(), collisions.size(), frameStart, broadEnd, midEnd, narrowEnd, responseEnd, moveEnd);
            }
            frame++;
        }

        // Process events
        if (!config.runInTestMode) {
            checkForInput(slowMotion, pause, quit, draw, drawOBBs, drawAABBs, camera, freeCamera, centeredCamera);
        }

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame, config.fps, !config.runInTestMode);

        SDL_GL_SwapWindow(window);
    }

    if (config.log || config.runInTestMode) outputCSV.close();
    return 0;
}

int runAllScenes(Config config) {
    list<string> sceneNames = list<string>{ "bouncy-things", "capsule-static-floor", "many-balls",
        "missile", "missile2", "objects-resting", "one-ball-many-capsules", "ramp", "two-simultaneous-collisions" };

    SDL_Window* window = initializeSDL();
    for (auto scene = sceneNames.begin(); scene != sceneNames.end(); ++scene) {
        config.sceneName = *scene;
        config.logOutputFile = *scene + "_test.csv";
        runSimulation(config, 300, window);
    }

    return 0;
}


int main(int argc, char* argv[]) {
    Config config = ConfigLoader().getConfig();
    if (config.runInTestMode) return runAllScenes(config);
    return runSimulation(config, -1, initializeSDL());
}
