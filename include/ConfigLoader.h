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

enum class BPAlgorithmChoice { none, bruteForce, sweepAndPrune };

struct Config {
	string sceneName;
	int fps;
	int numLatLongs;
	bool drawHalfWhite;
	bool useMidPhase;
	BPAlgorithmChoice bpAlgorithm;
	bool log;
	bool runInTestMode;
	string logOutputFile;
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