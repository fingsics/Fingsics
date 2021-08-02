#include "../include/ConfigLoader.h"

Config::Config(map<string, string> config) {
    fps = 60;
    numLatLongs = 10;
    log = false;
    runMode = RunMode::normal;
    numRuns = 10;
    numFramesPerRun = 300;
    useMidPhase = false;
    bpAlgorithm = BPAlgorithmChoice::none;
    drawHalfWhite = false;
    sceneName = "";
    logOutputFile = "";

    auto it = config.find("FPS");
    if (it != config.end()) {
        fps = stoi(it->second);
    }

    it = config.find("NUM_LAT_LONGS");
    if (it != config.end()) {
        numLatLongs = stoi(it->second);
    }

    it = config.find("DRAW_HALF_WHITE");
    if (it != config.end()) {
        drawHalfWhite = !it->second.compare("true");
    }

    it = config.find("USE_MID_PHASE");
    if (it != config.end()) {
        useMidPhase = !it->second.compare("true");
    }

    it = config.find("BROAD_PHASE");
    if (it != config.end()) {
        if (!it->second.compare("BF")) bpAlgorithm = BPAlgorithmChoice::bruteForce;
        else if (!it->second.compare("SAP")) bpAlgorithm = BPAlgorithmChoice::sweepAndPrune;
        else if (!it->second.compare("MTSAP")) bpAlgorithm = BPAlgorithmChoice::multithreadSweepAndPrune;
    }

    it = config.find("LOG");
    if (it != config.end()) {
        log = !it->second.compare("true");
    }

    it = config.find("RUN_MODE");
    if (it != config.end()) {
        if (!it->second.compare("NORMAL")) runMode = RunMode::normal;
        else if (!it->second.compare("TEST")) runMode = RunMode::test;
        else if (!it->second.compare("BENCHMARK")) runMode = RunMode::benchmark;
    }

    it = config.find("NUM_RUNS");
    if (it != config.end()) {
        numRuns = stoi(it->second);
    }

    it = config.find("NUM_FRAMES_PER_RUN");
    if (it != config.end()) {
        numFramesPerRun = stoi(it->second);
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

bool Config::isRunningOnNormalMode() {
    return runMode == RunMode::normal;
}

bool Config::isRunningOnBenchmarkMode() {
    return runMode == RunMode::benchmark;
}

bool Config::isRunningOnTestMode() {
    return runMode == RunMode::test;
}

bool Config::shouldLog() {
    return isRunningOnBenchmarkMode() || isRunningOnTestMode() || log;
}

string Config::getMPCDDescription() {
    if (useMidPhase) return "OBB";
    else return "None";
}

string Config::getBPCDDescription() {
    switch (bpAlgorithm) {
    case BPAlgorithmChoice::bruteForce:
        return "BF";
    case BPAlgorithmChoice::none:
        return "None";
    case BPAlgorithmChoice::sweepAndPrune:
        return "SAP";
    }
    return "None";
}

KeyValue::KeyValue(string key, string value) {
    this->key = key;
    this->value = value;
}

ConfigLoader::ConfigLoader() {}

Config ConfigLoader::getConfig() {
    string text;
    ifstream configFile("config.txt");
    map<string, string> config;

    while (getline(configFile, text)) {
        if (text.length() == 0 || text.at(0) == '#' || text.find("=") == string::npos) continue;
        KeyValue keyValue = splitString(text);
        config.insert(pair<string, string>(keyValue.key, keyValue.value));
    }

    configFile.close();

    return Config(config);
}

KeyValue ConfigLoader::splitString(string s) {
    string key, value;
    string delimiter = "=";
    size_t pos = 0;
    
    if ((pos = s.find(delimiter)) != string::npos) {
        key = s.substr(0, pos);
        value = s.substr(pos + delimiter.length(), s.length());
    }
    else {
        throw "Invalid configuration file";
    }

    return KeyValue(key, value);
}