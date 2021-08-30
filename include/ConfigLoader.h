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

enum class BPAlgorithmChoice { none, bruteForce, sweepAndPrune, multithreadSweepAndPrune };
enum class RunMode { normal, test, benchmark, recordVideo };

struct Config {
	string sceneName;
	int fps;
	int numLatLongs;
	int windowWidth;
	int windowHeight;
	bool drawHalfWhite;
	bool useMidPhase;
	BPAlgorithmChoice bpAlgorithm;
	bool log;
	RunMode runMode;
	int numRuns;
	int numFramesPerRun;
	string logOutputFile;
	bool isRunningOnNormalMode();
	bool isRunningOnTestMode();
	bool isRunningOnBenchmarkMode();
	bool isRunningOnRecordVideoMode();
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