#ifndef CONFLOADER_H
#define CONFLOADER_H

#include "Config.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <filesystem>

using namespace std;

struct KeyValue {
	string key;
	string value;
	KeyValue(string, string);
};

class ConfigLoader {
private:
	int screenResolutionWidth;
	int screenResolutionHeight;
	KeyValue splitString(string);
public:
	ConfigLoader(int, int);
	Config getConfig();
};

#endif