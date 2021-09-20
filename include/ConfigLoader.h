#ifndef CONFLOADER_H
#define CONFLOADER_H

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

enum class BPAlgorithmChoice { none, bruteForce, sweepAndPrune };
enum class RunMode { normal, test, benchmark, replay };

struct Config {
	string sceneName;
	int fps;
	int numLatLongs;
	int windowWidth;
	int windowHeight;
	bool recordVideo;
	bool recordData;
	bool useMidPhase;
	BPAlgorithmChoice bpAlgorithm;
	bool log;
	RunMode runMode;
	int numRuns;
	int stopAtFrame;
	string logOutputFile;
	bool shouldRecordVideo();
	bool shouldRecordData();
	bool shouldLog();
	string getMPCDDescription();
	string getBPCDDescription();
	Config(map<string, string>);
};

class ConfigLoader {
private:
	KeyValue splitString(string);
public:
	ConfigLoader();
	Config getConfig();
};

#endif