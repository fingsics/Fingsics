// Fingsics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../include/SceneRenderer.h";
#include "../include/XmlReader.h"
#include "../include/ConfigLoader.h"
#include "../include/SceneRecorder.h"
#include "../include/CenteredCamera.h"
#include "../include/FreeCamera.h"
#include "../include/BroadPhaseAlgorithm.h"
#include "../include/NoBPCD.h"
#include "../include/AABBBruteForce.h"
#include "../include/OBBBruteForce.h"
#include "../include/SweepAndPrune.h"
#include "../include/SAPAndOBBs.h"
#include "../include/NarrowPhaseAlgorithm.h"
#include "../include/CollisionResponseAlgorithm.h"
#include "../include/LoggingManager.h"
#include "../include/VideoRecorder.h"

#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include <thread>
#include <map>
#include <filesystem>

#define _USE_MATH_DEFINES
#define FPS_CAP 60

using namespace std;

SDL_Window* initializeSDL(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    atexit(SDL_Quit);

    SDL_Window* window = SDL_CreateWindow("Fingsics", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        cerr << "Failed to initialize view mode: " << SDL_GetError() << endl;
        exit(1);
    }
    SDL_GL_CreateContext(window);

    return window;
}

void checkForInput(bool& slowMotion, bool& pause, bool& quit, bool& draw, bool& drawOBBs, bool& drawAABBs, int& nframe, Camera*& camera, Camera* freeCamera, Camera* centeredCamera, Config config) {
    SDL_Event event;
    int xm, ym;

    SDL_GetMouseState(&xm, &ym);
    while (SDL_PollEvent(&event)) {
        camera->update(event);
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
            case SDLK_SPACE:
                pause = !pause;
                break;
            case SDLK_m:
                slowMotion = !slowMotion;
                break;
            case SDLK_f:
                draw = !draw;
                break;
            case SDLK_o:
                drawOBBs = !drawOBBs;
                break;
            case SDLK_i:
                drawAABBs = !drawAABBs;
                break;
            case SDLK_c:
                if (camera == centeredCamera) camera = freeCamera;
                else camera = centeredCamera;
                break;
            default:
                break;
            }
            break;
        }
        case SDL_KEYDOWN: {
            if (config.runMode != RunMode::replay) break;
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                nframe = max(0, nframe - 600);
                break;
            case SDLK_DOWN:
                nframe = min(config.stopAtFrame - 1, nframe + 600);
                break;
            case SDLK_LEFT:
                nframe = max(0, nframe - 60);
                break;
            case SDLK_RIGHT:
                nframe = min(config.stopAtFrame - 1, nframe + 60);
                break;
            case SDLK_COMMA:
                pause = true;
                nframe = max(0, nframe - 1);
                break;
            case SDLK_PERIOD:
                pause = true;
                nframe = min(config.stopAtFrame - 1, nframe + 1);
                break;
            default:
                break;
            }
            break;
        }
        default:
            break;
        }
    }
}

void manageFrameTime(clock_t &lastFrameTime, float &secondsSinceLastFrame, bool shouldSleep) {
    float minFrameTime = 1.0 / FPS_CAP;
    secondsSinceLastFrame = (float)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    if (secondsSinceLastFrame < minFrameTime && shouldSleep) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)((minFrameTime - secondsSinceLastFrame) * 1000)));
        secondsSinceLastFrame = (float)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    }
    lastFrameTime = clock();
}

SimulationResults* runSimulation(Config config, SDL_Window* window, string outputsFolder) {
    SimulationResults* results = config.shouldLog() ? new SimulationResults() : NULL;
    XmlReader xmlReader = XmlReader(config.sceneName + ".xml", config.numLatLongs);

    // Camera
    Camera* centeredCamera = new CenteredCamera();
    Camera* settingsCamera = xmlReader.getCamera();
    Camera* freeCamera = settingsCamera ? settingsCamera : new FreeCamera();
    Camera* camera = settingsCamera ? freeCamera : centeredCamera;

    // Program options
    bool quit = false;
    bool pause = config.runMode == RunMode::defaultMode || config.runMode == RunMode::replay;
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
    if (config.runMode == RunMode::replay) {
        pair<vector<Object*>, int> recordedScene = SceneRecorder("output\\" + config.replayName).importRecordedScene(config);
        objectsVector = recordedScene.first;
        config.stopAtFrame = recordedScene.second;
    }
    else objectsVector = xmlReader.getObjects();
    Object** objects = &objectsVector[0];
    int numObjects = objectsVector.size();

    if (config.shouldRecordScene() && config.stopAtFrame == -1) throw std::runtime_error("To record a scene, please specify a maximum number of frames (STOP_AT_FRAME value in the config file)");

    SceneRecorder* sceneRecorder = config.shouldRecordScene() ? new SceneRecorder(objects, numObjects, config.stopAtFrame, outputsFolder) : NULL;

    SceneRenderer sceneRenderer = SceneRenderer(objects, numObjects, config.numLatLongs, config.numLatLongs);

    // Collision detection algorithms
    BroadPhaseAlgorithm* broadPhaseAlgorithm = NULL;
    NarrowPhaseAlgorithm* narrowPhaseAlgorithm = NULL;
    if (config.runMode != RunMode::replay) {
        narrowPhaseAlgorithm = new NarrowPhaseAlgorithm();
        switch (config.bpAlgorithm) {
        case BPAlgorithmChoice::aabbBruteForce:
            broadPhaseAlgorithm = new AABBBruteForce();
            break;
        case BPAlgorithmChoice::obbBruteForce:
            broadPhaseAlgorithm = new OBBBruteForce();
            break;
        case BPAlgorithmChoice::sweepAndPrune:
            broadPhaseAlgorithm = new SweepAndPrune(objects, numObjects);
            break;
        case BPAlgorithmChoice::SAPAndOBBs:
            broadPhaseAlgorithm = new SAPAndOBBs(objects, numObjects);
            break;
        default:
            broadPhaseAlgorithm = new NoBPCD();
            break;
        }
    }

    // Logging
    chrono::system_clock::time_point frameStart, broadEnd, midEnd, narrowEnd, responseEnd, moveEnd;

    // Collision collections
    map<string, pair<Object*, Object*>> broadPhaseCollisions;
    map<string, Collision> collisions;

    // FPS drawing
    int fps = 0;
    chrono::system_clock::time_point lastFPSDrawTime = std::chrono::system_clock::now();

    // Video recording
    GLubyte* pixels = NULL;
    uint8_t* rgb = NULL;
    VideoRecorder* recorder = new VideoRecorder();
    if (config.shouldRecordVideo()) {
        if (!filesystem::is_directory("output") || !filesystem::exists("output")) filesystem::create_directory("output");
        if (!filesystem::is_directory(outputsFolder) || !filesystem::exists(outputsFolder)) filesystem::create_directory(outputsFolder);
        string fileName = outputsFolder + "\\" + "scene.mpg";
        recorder->ffmpeg_encoder_start(fileName.c_str(), FPS_CAP, config.windowWidth, config.windowHeight);
    }
    
    sceneRenderer.initializeOpenGL(config.windowWidth, config.windowHeight);

    while (!quit && (config.stopAtFrame == -1 || nframe < config.stopAtFrame)) {
        if (config.runMode == RunMode::replay) {
            for (int i = 0; i < numObjects; i++) objects[i]->goToFrame(nframe);
        }

        if (config.runMode == RunMode::defaultMode || config.runMode == RunMode::replay) {
            sceneRenderer.setupFrame();
            camera->lookAt();
            sceneRenderer.setLighting();
            sceneRenderer.drawAxis();

            // Draw objects
            for (int i = 0; i < numObjects; i++) sceneRenderer.drawObject(objects[i], drawOBBs, drawAABBs);
            if (!config.shouldRecordVideo()) sceneRenderer.drawFPSCounter(fps);
        }

        if (!pause) {
            // Record video
            if (config.shouldRecordVideo()) {
                recorder->ffmpeg_encoder_glread_rgb(&rgb, &pixels, config.windowWidth, config.windowHeight, nframe);
                recorder->ffmpeg_encoder_encode_frame(rgb);
            }

            // Record data
            if (config.shouldRecordScene()) {
                sceneRecorder->recordFrame(objects, numObjects, nframe);
            }

            // Compute next frame
            if (config.runMode != RunMode::replay) {
                if (config.shouldLog()) frameStart = std::chrono::system_clock::now();
                broadPhaseCollisions = broadPhaseAlgorithm->getCollisions(objects, numObjects);
                if (config.shouldLog()) broadEnd = std::chrono::system_clock::now();
                collisions = narrowPhaseAlgorithm->getCollisions(broadPhaseCollisions);
                if (config.shouldLog()) narrowEnd = std::chrono::system_clock::now();
                CollisionResponseAlgorithm::collisionResponse(collisions);
                if (config.shouldLog()) responseEnd = std::chrono::system_clock::now();
                CollisionResponseAlgorithm::moveObjects(objects, numObjects, 1.0 / FPS_CAP, slowMotion);
                if (config.shouldLog()) {
                    moveEnd = std::chrono::system_clock::now();
                    results->addFrameResults(broadPhaseCollisions.size(), collisions.size(), frameStart, broadEnd, narrowEnd, responseEnd, moveEnd);
                }
            }
            nframe++;
        }

        // Process events
        checkForInput(slowMotion, pause, quit, draw, drawOBBs, drawAABBs, nframe, camera, freeCamera, centeredCamera, config);

        if (config.runMode == RunMode::replay && (nframe >= config.stopAtFrame || nframe < 0)) {
            pause = true;
            nframe = min(config.stopAtFrame - 1, max(0, nframe));
        }

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame, config.runMode == RunMode::defaultMode || config.runMode == RunMode::replay);

        if ((float)chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastFPSDrawTime).count() / 1000.0 > 120) {
            lastFPSDrawTime = std::chrono::system_clock::now();
            fps = (int)(1.0 / timeSinceLastFrame) > FPS_CAP ? FPS_CAP : (int)(1.0 / timeSinceLastFrame);
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
    if (config.shouldRecordScene()) {
        sceneRecorder->storeRecordedData(nframe);
    }

    return results;
}

void runTestScenes(Config config, SDL_Window* window, string outputsFolder) {
    list<string> testSceneNames = list<string>{ "bouncy-things", "capsule-static-floor", "many-balls",
        "big-grid-3", "missile2", "objects-resting", "one-ball-many-capsules", "bowling", "lag", "ramp", "two-simultaneous-collisions" };

    if (!filesystem::is_directory("testing") || !filesystem::exists("testing")) filesystem::create_directory("testing");
    if (!filesystem::is_directory("testing\\results") || !filesystem::exists("testing\\results")) filesystem::create_directory("testing\\results");

    for (auto scene = testSceneNames.begin(); scene != testSceneNames.end(); ++scene) {
        config.sceneName = *scene;
        config.stopAtFrame = 300;
        SimulationResults* results = runSimulation(config, window, outputsFolder);
        if (results) {
            LoggingManager::logRunResults("testing\\results", *scene + "_test.csv", *results);
            delete results;
        }
    }
}

void runSceneBenchmark(Config config, SDL_Window* window, string outputsFolder) {
    list<SimulationResults> benchmarkResults = list<SimulationResults>();
    for (int i = 0; i < config.numRuns; i++) {
        SimulationResults* results = runSimulation(config, window, outputsFolder);
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

        time_t time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        std::tm* time2 = std::localtime(&time);
        string timeString = to_string(time2->tm_mday) + "-" + to_string(time2->tm_mon + 1) + "-" + to_string(time2->tm_year + 1900) + "-" + to_string(time2->tm_hour) + "_" + to_string(time2->tm_min);
        string outputsFolder = "output\\" + config.sceneName + "_" + timeString;

        SDL_Window* window = initializeSDL(config.windowWidth, config.windowHeight);

        if (config.runMode == RunMode::test) {
            runTestScenes(config, window, outputsFolder);
        }
        if (config.runMode == RunMode::benchmark) {
            runSceneBenchmark(config, window, outputsFolder);
        }
        if (config.runMode == RunMode::defaultMode) {
            SimulationResults* results = runSimulation(config, window, outputsFolder);
            if (results && config.log) {
                if (!filesystem::is_directory("output") || !filesystem::exists("output")) filesystem::create_directory("output");
                if (!filesystem::is_directory(outputsFolder) || !filesystem::exists(outputsFolder)) filesystem::create_directory(outputsFolder);
                LoggingManager::logRunResults(outputsFolder, "log.csv", *results);
                delete results;
            }
        }
        if (config.runMode == RunMode::replay) {
            runSimulation(config, window, outputsFolder);
        }
    //}
    //catch (const std::exception& ex) {
    //    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", ex.what(), NULL);
    //    return 1;
    //}
    //
    return 0;
}
