#include "../include/ConfigLoader.h"

Config::Config(map<string, string> config) {
    fps = 60;
    numLatLongs = 10;
    log = false;
    runInTestMode = false;
    useMidPhase = false;
    bpAlgorithm = BPAlgorithmChoice::sweepAndPrune;
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

    it = config.find("USE_MID_PHASE");
    if (it != config.end()) {
        useMidPhase = !it->second.compare("true");
    }

    it = config.find("BROAD_PHASE");
    if (it != config.end()) {
        if (!it->second.compare("SAP")) bpAlgorithm = BPAlgorithmChoice::sweepAndPrune;
        else if (!it->second.compare("BF")) bpAlgorithm = BPAlgorithmChoice::bruteForce;
        else if (!it->second.compare("NONE")) bpAlgorithm = BPAlgorithmChoice::none;
    }

    it = config.find("LOG");
    if (it != config.end()) {
        log = !it->second.compare("true");
    }

    it = config.find("RUN_IN_TEST_MODE");
    if (it != config.end()) {
        runInTestMode = !it->second.compare("true");
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