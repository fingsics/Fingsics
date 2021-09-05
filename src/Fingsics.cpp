// Fingsics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../include/SDLHelpers.h";
#include "../include/OpenGlHelpers.h";
#include "../include/XmlReader.h"
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
#include "../include/VideoRecorder.h"
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
    XmlReader xmlReader = XmlReader(config.sceneName + ".xml", config.numLatLongs);

    // Camera
    Camera* centeredCamera = new CenteredCamera();
    Camera* settingsCamera = xmlReader.getCamera();
    Camera* freeCamera = settingsCamera ? settingsCamera : new FreeCamera();
    Camera* camera = settingsCamera ? freeCamera : centeredCamera;

    // Program options
    bool quit = false;
    bool pause = config.runMode == RunMode::normal || config.runMode == RunMode::replay;
    bool draw = true;
    bool slowMotion = false;
    bool drawOBBs = false;
    bool drawAABBs = false;

    // FPS management
    clock_t lastFrameTime = clock();
    float timeSinceLastFrame = 0;
    int nframe = 0;

    // Scene
    vector<Object*> objectsVector;
    if (config.runMode == RunMode::replay) objectsVector = SceneRecorder(config.sceneName + ".dat").importRecordedScene(config);
    else objectsVector = xmlReader.getObjects();
    Object** objects = &objectsVector[0];
    int numObjects = objectsVector.size();
    SceneRecorder* sceneRecorder = config.shouldRecordData() ? new SceneRecorder(objects, numObjects, config.stopAtFrame, config.sceneName + ".dat") : NULL;

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

    // Collision collections
    map<string, pair<Object*, Object*>> broadPhaseCollisions, midPhaseCollisions;
    map<string, Collision> collisions;

    // FPS drawing
    int fps = 0;
    chrono::system_clock::time_point lastFPSDrawTime = std::chrono::system_clock::now();

    // Video recording
    GLubyte* pixels = NULL;
    uint8_t* rgb = NULL;
    VideoRecorder* recorder = new VideoRecorder();
    if (config.shouldRecordVideo()) {
        if (!filesystem::is_directory("recordings") || !filesystem::exists("recordings")) filesystem::create_directory("recordings");
        string fileName = "recordings\\" + config.sceneName + ".mpg";
        recorder->ffmpeg_encoder_start(fileName.c_str(), config.fps, config.windowWidth, config.windowHeight);
    }
    
    while (!quit && (config.stopAtFrame == -1 || nframe < config.stopAtFrame)) {
        if (config.runMode == RunMode::replay) {
            for (int i = 0; i < numObjects; i++) objects[i]->goToFrame(nframe);
        }

        if (config.runMode == RunMode::normal || config.runMode == RunMode::replay) {
            setupFrame();
            camera->lookAt();
            setLighting();

            drawAxis();
            // Draw objects
            for (int i = 0; i < numObjects; i++) objects[i]->draw(drawOBBs, drawAABBs, config.drawHalfWhite);
            if (!config.shouldRecordVideo()) drawFPSCounter(fps);
        }

        if (!pause) {
            // Record video
            if (config.shouldRecordVideo()) {
                recorder->ffmpeg_encoder_glread_rgb(&rgb, &pixels, config.windowWidth, config.windowHeight, nframe);
                recorder->ffmpeg_encoder_encode_frame(rgb);
            }

            // Record data
            if (config.shouldRecordData()) {
                sceneRecorder->recordFrame(objects, numObjects, nframe);
            }

            // Compute next frame
            if (config.runMode != RunMode::replay) {
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
            }
            nframe++;
        }

        // Process events
        checkForInput(
            slowMotion,
            pause,
            quit,
            draw,
            drawOBBs,
            drawAABBs,
            nframe,
            camera,
            freeCamera,
            centeredCamera,
            config
        );

        if (config.runMode == RunMode::replay && (nframe >= config.stopAtFrame || nframe < 0)) {
            pause = true;
            nframe = min(config.stopAtFrame - 1, max(0, nframe));
        }

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame, config.fps, config.runMode == RunMode::normal || config.runMode == RunMode::replay);

        if ((float)chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastFPSDrawTime).count() / 1000.0 > 120) {
            lastFPSDrawTime = std::chrono::system_clock::now();
            fps = (int)(1.0 / timeSinceLastFrame) > config.fps ? config.fps : (int)(1.0 / timeSinceLastFrame);
        }

        SDL_GL_SwapWindow(window);
    }

    // Store video
    if (config.shouldRecordVideo()) {
        recorder->ffmpeg_encoder_finish();
        free(pixels);
        free(rgb);
    }

    // Store data
    if (config.shouldRecordData()) {
        sceneRecorder->storeRecordedData();
    }

    return results;
}

void runTestScenes(Config config, SDL_Window* window) {
    list<string> testSceneNames = list<string>{ "bouncy-things", "capsule-static-floor", "many-balls",
        "big-grid-3", "missile2", "objects-resting", "one-ball-many-capsules", "bowling", "lag", "ramp", "two-simultaneous-collisions" };

    if (!filesystem::is_directory("testing") || !filesystem::exists("testing")) filesystem::create_directory("testing");
    if (!filesystem::is_directory("testing\\results") || !filesystem::exists("testing\\results")) filesystem::create_directory("testing\\results");

    for (auto scene = testSceneNames.begin(); scene != testSceneNames.end(); ++scene) {
        config.sceneName = *scene;
        SimulationResults* results = runSimulation(config, window);
        if (results) {
            LoggingManager::logRunResults("testing\\results", *scene + "_test.csv", *results);
            delete results;
        }
    }
}

void runSceneBenchmark(Config config, SDL_Window* window) {
    list<SimulationResults> benchmarkResults = list<SimulationResults>();
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

        SDL_Window* window = initializeSDL(config.windowWidth, config.windowHeight);
        initializeOpenGL();

        if (config.runMode == RunMode::test) {
            runTestScenes(config, window);
        }
        if (config.runMode == RunMode::benchmark) {
            runSceneBenchmark(config, window);
        }
        if (config.runMode == RunMode::normal) {
            SimulationResults* results = runSimulation(config, window);
            if (results && config.log) {
                if (!filesystem::is_directory("output") || !filesystem::exists("output")) filesystem::create_directory("output");
                LoggingManager::logRunResults("output", config.logOutputFile, *results);
                delete results;
            }
        }
        if (config.runMode == RunMode::replay) {
            runSimulation(config, window);
        }
    //}
    //catch (const std::exception& ex) {
    //    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", ex.what(), NULL);
    //    return 1;
    //}
    //
    return 0;
}
