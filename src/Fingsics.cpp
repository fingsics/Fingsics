// Fingsics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../include/SDLHelpers.h";
#include "../include/OpenGlHelpers.h";
#include "../include/ObjectLoader.h"
#include "../include/ConfigLoader.h"
#include "../include/SceneRecorder.h"
#include "../include/CenteredCamera.h"
#include "../include/FreeCamera.h"
#include "../include/BroadPhaseAlgorithms.h"
#include "../include/NoBroadPhase.h"
#include "../include/BruteForceBroadPhase.h"
#include "../include/SweepAndPruneBroadPhase.h"
#include "../include/MultithreadSweepAndPruneBroadPhase.h"
#include "../include/MidPhaseAlgorithms.h"
#include "../include/NarrowPhaseAlgorithm.h"
#include "../include/CollisionResponseAlgorithm.h"
#include "../include/LoggingManager.h"
#include <iostream>
#include <thread>
#include <map>
#include <filesystem>

#define _USE_MATH_DEFINES

using namespace std;


void manageFrameTime(clock_t &lastFrameTime, float &secondsSinceLastFrame, int fps, bool shouldSleep) {
    float minFrameTime = 1.0 / fps;
    secondsSinceLastFrame = (float)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    if (secondsSinceLastFrame < minFrameTime && shouldSleep) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)((minFrameTime - secondsSinceLastFrame) * 1000)));
        secondsSinceLastFrame = (float)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    }
    lastFrameTime = clock();
}

SimulationResults* runSimulation(Config config, SDL_Window* window) {
    SimulationResults* results = config.shouldLog() ? new SimulationResults() : NULL;

    // Camera
    Camera* centeredCamera = new CenteredCamera();
    Camera* freeCamera = new FreeCamera();
    Camera* camera = centeredCamera;

    // Program options
    bool quit = false;
    bool pause = config.isRunningOnNormalMode();
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
    case BPAlgorithmChoice::bruteForce:
        broadPhaseAlgorithm = new BruteForceBroadPhase();
        break;
    case BPAlgorithmChoice::sweepAndPrune:
        broadPhaseAlgorithm = new SweepAndPruneBroadPhase(objects, numObjects);
        break;
    case BPAlgorithmChoice::multithreadSweepAndPrune:
        broadPhaseAlgorithm = new MultithreadSweepAndPruneBroadPhase(objects, numObjects);
        break;
    default:
        broadPhaseAlgorithm = new NoBroadPhase();
        break;
    }

    // Logging
    chrono::system_clock::time_point frameStart, broadEnd, midEnd, narrowEnd, responseEnd, moveEnd;



    initializeOpenGL();

    // Collision collections
    map<string, pair<Object*, Object*>> broadPhaseCollisions, midPhaseCollisions;
    map<string, Collision> collisions;

    // FPS drawing
    int fps = 0;
    chrono::system_clock::time_point lastFPSDrawTime = std::chrono::system_clock::now();

    while (!quit && (config.isRunningOnNormalMode() || frame < config.numFramesPerRun)) {
        if (config.isRunningOnNormalMode()) {
            setupFrame();
            camera->lookAt();
            setLighting();
        }

        // Draw objects
        if (draw && config.isRunningOnNormalMode()) {
            drawAxis();
            drawObjects(objects, numObjects, drawOBBs, drawAABBs, config.drawHalfWhite);
            drawFPSCounter(fps);
        }

        // Apply physics and movement
        if (!pause) {
            if (config.shouldLog()) frameStart = std::chrono::system_clock::now();
            broadPhaseCollisions = broadPhaseAlgorithm->getCollisions(objects, numObjects);
            if (config.shouldLog()) broadEnd = std::chrono::system_clock::now();
            midPhaseCollisions = midPhaseAlgorithm->getCollisions(broadPhaseCollisions);
            if (config.shouldLog()) midEnd = std::chrono::system_clock::now();
            collisions = narrowPhaseAlgorithm->getCollisions(midPhaseCollisions);
            if (config.shouldLog()) narrowEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::collisionResponse(collisions);
            if (config.shouldLog()) responseEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::moveObjects(objects, numObjects, 1.0 / config.fps, slowMotion);
            if (config.shouldLog()) {
                moveEnd = std::chrono::system_clock::now();
                results->addFrameResults(broadPhaseCollisions.size(), midPhaseCollisions.size(), collisions.size(), frameStart, broadEnd, midEnd, narrowEnd, responseEnd, moveEnd);
            }
            frame++;
            if (config.isRunningOnRecorderMode()) {

            }
        }

        // Process events
        if (config.isRunningOnNormalMode()) {
            checkForInput(slowMotion, pause, quit, draw, drawOBBs, drawAABBs, camera, freeCamera, centeredCamera);
        }

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame, config.fps, config.isRunningOnNormalMode());

        if ((float)chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastFPSDrawTime).count() / 1000.0 > 120) {
            lastFPSDrawTime = std::chrono::system_clock::now();
            fps = (int)(1.0 / timeSinceLastFrame) > config.fps ? config.fps : (int)(1.0 / timeSinceLastFrame);
        }

        SDL_GL_SwapWindow(window);
    }

    return results;
}

void runTestScenes(Config config) {
    list<string> testSceneNames = list<string>{ "bouncy-things", "capsule-static-floor", "many-balls",
        "big-grid-3", "missile2", "objects-resting", "one-ball-many-capsules", "bowling", "lag", "ramp", "two-simultaneous-collisions" };

    if (!filesystem::is_directory("testing") || !filesystem::exists("testing")) filesystem::create_directory("testing");
    if (!filesystem::is_directory("testing\\results") || !filesystem::exists("testing\\results")) filesystem::create_directory("testing\\results");
    SDL_Window* window = initializeSDL();

    for (auto scene = testSceneNames.begin(); scene != testSceneNames.end(); ++scene) {
        config.sceneName = *scene;
        SimulationResults* results = runSimulation(config, window);
        if (results) {
            LoggingManager::logRunResults("testing\\results", *scene + "_test.csv", *results);
            delete results;
        }
    }
}

void runSceneBenchmark(Config config) {
    list<SimulationResults> benchmarkResults = list<SimulationResults>();
    SDL_Window* window = initializeSDL();
    for (int i = 0; i < config.numRuns; i++) {
        SimulationResults* results = runSimulation(config, window);
        if (results) {
            benchmarkResults.push_back(*results);
            delete results;
        }
    }
    LoggingManager::logBenchmarkResults(benchmarkResults, config);
}

int main(int argc, char* argv[]) {
   // try {
        Config config = ConfigLoader().getConfig();

        if (config.isRunningOnTestMode()) {
            runTestScenes(config);
        }
        else if (config.isRunningOnBenchmarkMode()) {
            runSceneBenchmark(config);
        }
        else {
            SimulationResults* results = runSimulation(config, initializeSDL());
            if (results && config.log) {
                if (!filesystem::is_directory("output") || !filesystem::exists("output")) filesystem::create_directory("output");
                LoggingManager::logRunResults("output", config.logOutputFile, *results);
                delete results;
            }
        }
    //}
    //catch (const std::exception& ex) {
    //    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", ex.what(), NULL);
    //    return 1;
    //}
    //
    return 0;
}
