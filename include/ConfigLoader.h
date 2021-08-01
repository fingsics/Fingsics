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
enum class RunMode { normal, test, benchmark };


struct Config {
	string sceneName;
	int fps;
	int numLatLongs;
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

#endif CONFLOADER_H