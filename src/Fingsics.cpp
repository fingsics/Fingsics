// Fingsics.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../include/SceneRenderer.h";
#include "../include/XmlReader.h"
#include "../include/ConfigLoader.h"
#include "../include/SceneRecorder.h"
#include "../include/CenteredCamera.h"
#include "../include/FreeCamera.h"
#include "../include/Scene.h"
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

const int microsecondsInOneSecond = 1000000;

pair<int,int> getResolution() {    
    HWND hwnd = GetDesktopWindow();
    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    int width = info.rcMonitor.right - info.rcMonitor.left;
    int height = info.rcMonitor.bottom - info.rcMonitor.top;
    return pair(width, height);
}

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

void checkForInput(bool& slowMotion, bool& pause, bool& quit, bool& drawOBBs, bool& drawAABBs, int& nframe, Camera*& camera, Camera* freeCamera, Camera* centeredCamera, Config config, int stopAtFrame) {
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
                nframe = min(stopAtFrame - 1, nframe + 600);
                break;
            case SDLK_LEFT:
                nframe = max(0, nframe - 60);
                break;
            case SDLK_RIGHT:
                nframe = min(stopAtFrame - 1, nframe + 60);
                break;
            case SDLK_COMMA:
                pause = true;
                nframe = max(0, nframe - 1);
                break;
            case SDLK_PERIOD:
                pause = true;
                nframe = min(stopAtFrame - 1, nframe + 1);
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

void drawScene(SceneRenderer sceneRenderer, Camera* camera, vector<Object*> objects, bool drawOBBs, bool drawAABBs, bool drawAxes) {
    sceneRenderer.setupFrame();
    camera->lookAt();
    sceneRenderer.setLighting();
    if (drawAxes) sceneRenderer.drawAxis();
    for (int i = 0; i < objects.size(); i++) sceneRenderer.drawObject(objects[i], drawOBBs, drawAABBs);
}

void recordVideoFrame(VideoRecorder* recorder, Config config, GLubyte* pixels, uint8_t* rgb, int nframe) {
    recorder->ffmpeg_encoder_glread_rgb(&rgb, &pixels, config.windowWidth, config.windowHeight, nframe);
    recorder->ffmpeg_encoder_encode_frame(rgb);
}

void handleFPS(int fpsCap, chrono::system_clock::time_point frameStart, int& rollingAvgFrametime) {
    const float newFrameWeight = 0.05;
    int lastFrameTime = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - frameStart).count();
    rollingAvgFrametime = (1 - newFrameWeight) * (float)rollingAvgFrametime + newFrameWeight * (float)lastFrameTime;
}

BroadPhaseAlgorithm* getBroadPhaseAlgorithm(Config config, vector<Object*> objects) {
    switch (config.bpAlgorithm) {
    case BPAlgorithmChoice::aabbBruteForce: return new AABBBruteForce();
    case BPAlgorithmChoice::obbBruteForce: return new OBBBruteForce();
    case BPAlgorithmChoice::sweepAndPrune: return new SweepAndPrune(objects);
    case BPAlgorithmChoice::SAPAndOBBs: return new SAPAndOBBs(objects);
    default: return new NoBPCD();
    }
}

Scene getScene(Config config) {
    Camera* centeredCamera = new FreeCamera();
    Camera* settingsCamera = NULL;
    vector<Object*> objects;
    int stopAtFrame = config.stopAtFrame;
    int fpsCap = config.fpsCap;

    if (config.runMode == RunMode::replay) {
        tuple<vector<Object*>, int, int> recordedScene = SceneRecorder("output\\" + config.replayName).importRecordedScene(config);
        objects = get<0>(recordedScene);
        stopAtFrame = get<1>(recordedScene);
        fpsCap = get<2>(recordedScene);
    }
    else {
        XmlReader xmlReader = XmlReader(config.sceneName + ".xml", config.numLatLongs);
        settingsCamera = xmlReader.getCamera();
        objects = xmlReader.getObjects();
    }

    centeredCamera = settingsCamera ? settingsCamera : new CenteredCamera();
    
    return Scene(objects, centeredCamera, new FreeCamera(), centeredCamera, stopAtFrame, fpsCap);
}

SimulationResults* runSimulation(Config config, SDL_Window* window, string outputsFolder) {
    SimulationResults* results = config.shouldLog() ? new SimulationResults() : NULL;

    // Initialize scene
    Scene scene = getScene(config);

    // Program options
    bool quit = false;
    bool pause = config.runMode == RunMode::defaultMode || config.runMode == RunMode::replay;
    bool slowMotion = false;
    bool drawOBBs = false;
    bool drawAABBs = false;

    // Variables
    map<string, pair<Object*, Object*>> broadPhaseCollisions;
    map<string, Collision> collisions;
    NPCDData npcdData = NPCDData();
    int nframe = 0;
    int currentReplayFrame = 0;
    int rollingAvgFrametime = microsecondsInOneSecond / scene.fpsCap; // Microseconds

    // Initialize scene recorder
    if (config.shouldRecordScene() && scene.stopAtFrame == -1) throw std::runtime_error("To record a scene, please specify a maximum number of frames (STOP_AT_FRAME value in the config file)");
    SceneRecorder* sceneRecorder = config.shouldRecordScene() ? new SceneRecorder(scene.objects, scene.stopAtFrame, outputsFolder) : NULL;

    // Initialize renderer
    SceneRenderer sceneRenderer = SceneRenderer(scene.objects, config.numLatLongs, config.numLatLongs);
    sceneRenderer.initializeOpenGL(config.windowWidth, config.windowHeight);

    // Initialize collision detection algorithms
    BroadPhaseAlgorithm* broadPhaseAlgorithm = NULL;
    NarrowPhaseAlgorithm* narrowPhaseAlgorithm = NULL;
    if (config.runMode != RunMode::replay) {
        narrowPhaseAlgorithm = new NarrowPhaseAlgorithm();
        broadPhaseAlgorithm = getBroadPhaseAlgorithm(config, scene.objects);
    }

    // Initiailize video recording
    GLubyte* pixels = NULL;
    uint8_t* rgb = NULL;
    VideoRecorder* recorder = new VideoRecorder();
    if (config.shouldRecordVideo()) {
        if (!filesystem::is_directory("output") || !filesystem::exists("output")) filesystem::create_directory("output");
        if (!filesystem::is_directory(outputsFolder) || !filesystem::exists(outputsFolder)) filesystem::create_directory(outputsFolder);
        string fileName = outputsFolder + "\\" + "scene.mpg";
        recorder->ffmpeg_encoder_start(fileName.c_str(), scene.fpsCap < 60 ? scene.fpsCap : 60, config.windowWidth, config.windowHeight);
    }

    // Draw and record first frame
    if (config.shouldDrawScene()) {
        drawScene(sceneRenderer, scene.currentCamera, scene.objects, drawOBBs, drawAABBs, config.drawAxes);
        if (config.shouldRecordVideo()) recordVideoFrame(recorder, config, pixels, rgb, nframe);
    }
    if (config.shouldRecordScene()) sceneRecorder->recordFrame(scene.objects, nframe);
    nframe++;

    // Simulation time management
    chrono::system_clock::time_point collHandStart, broadEnd, midEnd, narrowEnd, responseEnd, drawStart, drawEnd;
    int stepInMicroseconds = microsecondsInOneSecond / scene.fpsCap;
    chrono::microseconds step = chrono::microseconds(stepInMicroseconds);
    chrono::system_clock::time_point frameStart, frameEnd;
    frameEnd = chrono::system_clock::now();

    // ----- MAIN LOOP -----
    while (!quit && (scene.stopAtFrame == -1 || nframe < scene.stopAtFrame)) {
        frameStart = chrono::system_clock::now();
        frameEnd = frameStart + step;

        // Compute next frame
        if (!pause && config.runMode != RunMode::replay) {
            if (config.shouldLog()) collHandStart = std::chrono::system_clock::now();
            broadPhaseCollisions = broadPhaseAlgorithm->getCollisions(scene.objects);
            if (config.shouldLog()) broadEnd = std::chrono::system_clock::now();
            pair<map<string, Collision>, NPCDData> npcdResults = narrowPhaseAlgorithm->getCollisions(broadPhaseCollisions);
            collisions = npcdResults.first;
            npcdData = npcdResults.second;
            if (config.shouldLog()) narrowEnd = std::chrono::system_clock::now();
            CollisionResponseAlgorithm::collisionResponse(collisions);
            if (config.shouldLog()) responseEnd = std::chrono::system_clock::now();

            CollisionResponseAlgorithm::moveObjects(scene.objects, 1.0 / scene.fpsCap, slowMotion);
        }

        // Replay handling
        if (config.runMode == RunMode::replay) {
            if (currentReplayFrame != nframe) for (int i = 0; i < scene.objects.size(); i++) scene.objects[i]->goToFrame(nframe);
            currentReplayFrame = nframe;
        }

        // Draw scene
        if (config.shouldDrawScene()) {
            if (config.shouldLog()) drawStart = std::chrono::system_clock::now();
            drawScene(sceneRenderer, scene.currentCamera, scene.objects, drawOBBs, drawAABBs, config.drawAxes);
            if (config.shouldLog()) drawEnd = std::chrono::system_clock::now();
        }
        else if (config.shouldLog()) {
            drawStart = std::chrono::system_clock::now();
            drawEnd = drawStart;
        }

        // Persist data
        if (!pause) {
            if (config.shouldRecordVideo() && (config.fpsCap != 120 || nframe % 2 == 1)) recordVideoFrame(recorder, config, pixels, rgb, nframe);
            if (config.shouldRecordScene()) sceneRecorder->recordFrame(scene.objects, nframe);
            if (config.shouldLog()) results->addFrameResults(broadPhaseCollisions.size(), collisions.size(), collHandStart, broadEnd, narrowEnd, responseEnd, drawStart, drawEnd, npcdData);
        }

        // Draw curent FPS
        if (config.shouldDrawScene() && config.showFPS) {
            handleFPS(scene.fpsCap, frameStart, rollingAvgFrametime);
            int fps = microsecondsInOneSecond / rollingAvgFrametime > scene.fpsCap ? scene.fpsCap : microsecondsInOneSecond / rollingAvgFrametime;
            sceneRenderer.drawFPSCounter(fps);
        }

        if (!pause) {
            if (config.runMode != RunMode::replay) nframe++;
            else nframe = min(scene.stopAtFrame - 1, max(0, nframe + 1));
        }

        // Process events
        checkForInput(slowMotion, pause, quit, drawOBBs, drawAABBs, nframe, scene.currentCamera, scene.freeCamera, scene.centeredCamera, config, scene.stopAtFrame);

        SDL_GL_SwapWindow(window);

        // Sleep if necessary
        if ((config.runMode == RunMode::defaultMode || config.runMode == RunMode::replay) && chrono::system_clock::now() < frameEnd)
            this_thread::sleep_until(frameEnd);
    }

    // Store video recording
    if (config.shouldRecordVideo()) {
        recorder->ffmpeg_encoder_finish();
        free(pixels);
        free(rgb);
    }

    // Store scene recording
    if (config.shouldRecordScene()) {
        sceneRecorder->storeRecordedData(nframe, scene.fpsCap);
    }

    return results;
}

void runTestScenes(Config config, SDL_Window* window, string outputsFolder) {
    list<string> testSceneNames = list<string>{ "many-balls", "missile", "missile2",
        "objects-resting", "bowling", "matrix", "ramp", "simultaneous-collisions" };

    if (!filesystem::is_directory("testing") || !filesystem::exists("testing")) filesystem::create_directory("testing");
    if (!filesystem::is_directory("testing\\results") || !filesystem::exists("testing\\results")) filesystem::create_directory("testing\\results");

    config.stopAtFrame = 300;
    for (auto scene = testSceneNames.begin(); scene != testSceneNames.end(); ++scene) {
        config.sceneName = *scene;
        SimulationResults* results = runSimulation(config, window, outputsFolder);
        if (results) {
            LoggingManager::logRunResults("testing\\results", *scene + "_test.csv", *results);
            delete results;
        }
    }
}

void runSceneBenchmark(Config config, SDL_Window* window, string outputsFolder) {
    list<SimulationResults> benchmarkResults = list<SimulationResults>();

    if (config.stopAtFrame == -1) throw std::runtime_error("To do a benchmark run, please specify a maximum number of frames (STOP_AT_FRAME value in the config file)");

    for (int i = 0; i < config.numRuns; i++) {
        SimulationResults* results = runSimulation(config, window, outputsFolder);
        if (results) {
            benchmarkResults.push_back(*results);
            delete results;
        }
    }
    LoggingManager::logBenchmarkResults(benchmarkResults, config, outputsFolder);
}

string getOutputsFolderName(Config config) {
    time_t clock = chrono::system_clock::to_time_t(chrono::system_clock::now());
    std::tm* time = std::localtime(&clock);
    char timeChars[32];
    int timeLength = std::strftime(timeChars, sizeof(timeChars), "%Y-%m-%d--%H-%M-%S", time);
    if (config.runMode == RunMode::benchmark) {
        return "benchmarks\\" + config.sceneName + "_" + config.getBPCDDescription() + "--" + string(timeChars, timeLength);
    }
    return "output\\" + config.sceneName + "--" + string(timeChars, timeLength);
}

int main(int argc, char* argv[]) {
   try {
        pair<int, int> resolution = getResolution();
        Config config = ConfigLoader(resolution.first, resolution.second).getConfig();
        SDL_Window* window = initializeSDL(config.windowWidth, config.windowHeight);
        string outputsFolder = getOutputsFolderName(config);

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
    }
    catch (const std::exception& ex) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", ex.what(), NULL);
        return 1;
    }
    
    return 0;
}
