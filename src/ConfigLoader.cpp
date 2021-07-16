#include "../include/ConfigLoader.h"

Config::Config(map<string, string> config) {
    int fps = 60;
    int numLatLongs = 10;
    bool log = false;
    string sceneName = "";
    string logOutputFile = "";

    auto it = config.find("FPS");
    if (it != config.end()) {
        fps = stoi(it->second);
    }

    it = config.find("NUM_LAT_LONGS");
    if (it != config.end()) {
        numLatLongs = stoi(it->second);
    }

    it = config.find("LOG");
    if (it != config.end()) {
        log = !it->second.compare("true");
    }

    it = config.find("SCENE_FILE_NAME");
    if (it != config.end()) {
        sceneName = it->second;
    }

    it = config.find("LOG_OUTPUT_FILE_NAME");
    if (it != config.end()) {
        logOutputFile = it->second;
    }

    this->fps = fps;
    this->numLatLongs = numLatLongs;
    this->log = log;
    this->sceneName = sceneName;
    this->logOutputFile = logOutputFile;
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

    return KeyValue(key, value);
}