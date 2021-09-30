#include "../include/Config.h"

using namespace std;

Config::Config(map<string, string> config) {
    fps = 60;
    windowWidth = 1280;
    windowHeight = 720;
    recordVideo = false;
    recordData = false;
    numLatLongs = 8;
    log = false;
    runMode = RunMode::defaultMode;
    numRuns = 10;
    stopAtFrame = 300;
    bpAlgorithm = BPAlgorithmChoice::none;
    sceneName = "";
    logOutputFile = "";

    auto it = config.find("FPS");
    if (it != config.end()) {
        fps = stoi(it->second);
    }

    it = config.find("WINDOW_WIDTH");
    if (it != config.end()) {
        windowWidth = stoi(it->second);
    }

    it = config.find("WINDOW_HEIGHT");
    if (it != config.end()) {
        windowHeight = stoi(it->second);
    }

    it = config.find("RECORD_VIDEO");
    if (it != config.end()) {
        recordVideo = !it->second.compare("true");
    }

    it = config.find("RECORD_DATA");
    if (it != config.end()) {
        recordData = !it->second.compare("true");
    }

    it = config.find("NUM_LAT_LONGS");
    if (it != config.end()) {
        numLatLongs = stoi(it->second);
    }

    it = config.find("BROAD_PHASE");
    if (it != config.end()) {
        if (!it->second.compare("NONE")) bpAlgorithm = BPAlgorithmChoice::none;
        else if (!it->second.compare("AABB")) bpAlgorithm = BPAlgorithmChoice::aabbBruteForce;
        else if (!it->second.compare("OBB")) bpAlgorithm = BPAlgorithmChoice::obbBruteForce;
        else if (!it->second.compare("SAP")) bpAlgorithm = BPAlgorithmChoice::sweepAndPrune;
        else if (!it->second.compare("SAPOBB")) bpAlgorithm = BPAlgorithmChoice::SAPAndOBBs;
    }

    it = config.find("LOG");
    if (it != config.end()) {
        log = !it->second.compare("true");
    }

    it = config.find("RUN_MODE");
    if (it != config.end()) {
        if (!it->second.compare("TEST")) runMode = RunMode::test;
        else if (!it->second.compare("BENCHMARK")) runMode = RunMode::benchmark;
        else if (!it->second.compare("REPLAY")) runMode = RunMode::replay;
    }

    it = config.find("NUM_RUNS_FOR_BENCHMARK");
    if (it != config.end()) {
        numRuns = stoi(it->second);
    }

    it = config.find("STOP_AT_FRAME");
    if (it != config.end()) {
        stopAtFrame = stoi(it->second);
    }

    it = config.find("SCENE_FILE_NAME");
    if (it != config.end()) {
        sceneName = it->second;
    }

    it = config.find("LOG_OUTPUT_FILE_NAME");
    if (it != config.end()) {
        logOutputFile = it->second;
    }
}

bool Config::shouldRecordVideo() {
    return runMode == RunMode::defaultMode && recordVideo;
}

bool Config::shouldRecordData() {
    return runMode == RunMode::defaultMode && recordData;
}

bool Config::shouldLog() {
    return runMode == RunMode::benchmark || runMode == RunMode::test || log;
}

string Config::getBPCDDescription() {
    switch (bpAlgorithm) {
    case BPAlgorithmChoice::none:
        return "None";
    case BPAlgorithmChoice::aabbBruteForce:
        return "AABB";
    case BPAlgorithmChoice::obbBruteForce:
        return "OBB";
    case BPAlgorithmChoice::sweepAndPrune:
        return "SAP";
    case BPAlgorithmChoice::SAPAndOBBs:
        return "SAPOBB";
    }
    return "None";
}