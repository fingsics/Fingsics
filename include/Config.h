#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <filesystem>

using namespace std;

enum class BPAlgorithmChoice { none, bruteForce, sweepAndPrune };
enum class RunMode { defaultMode, test, benchmark, replay };

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

#endif
