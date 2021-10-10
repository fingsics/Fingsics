#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <filesystem>

using namespace std;

enum class BPAlgorithmChoice { none, aabbBruteForce, obbBruteForce, sweepAndPrune, SAPAndOBBs};
enum class RunMode { defaultMode, test, benchmark, replay };

struct Config {
	string sceneName;
	int fpsCap;
	int numLatLongs;
	int windowWidth;
	int windowHeight;
	bool showFPS;
	bool recordVideo;
	bool recordScene;
	string replayName;
	BPAlgorithmChoice bpAlgorithm;
	bool log;
	RunMode runMode;
	int numRuns;
	int stopAtFrame;
	bool shouldRecordVideo();
	bool shouldRecordScene();
	bool shouldLog();
	string getBPCDDescription();
	Config(map<string, string>);
};

#endif
