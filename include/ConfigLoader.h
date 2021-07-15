#ifndef CONFLOADER_H
#define CONFLOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;

struct KeyValue {
	string key;
	string value;
	KeyValue(string, string);
};

struct Config {
	int fps;
	int numLatLongs;
	Config(map<string, string>);
};

class ConfigLoader {
private:
	KeyValue splitString(string);
public:
	ConfigLoader();
	Config getConfig();
};

#endif CONFLOADER_H