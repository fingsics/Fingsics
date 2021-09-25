#include "../include/ConfigLoader.h"

KeyValue::KeyValue(string key, string value) {
    this->key = key;
    this->value = value;
}

ConfigLoader::ConfigLoader() {}

Config ConfigLoader::getConfig() {
    string configFileName = "config.txt";
    if (!filesystem::exists(configFileName) || !filesystem::is_regular_file(configFileName)) throw std::runtime_error("The config.txt file is missing");

    string text;
    ifstream configFile(configFileName);
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
        throw std::runtime_error("Invalid configuration file");
    }

    return KeyValue(key, value);
}