#ifndef SIMRESULTS_H
#define SIMRESULTS_H

#include "Config.h"
#include "NPCDData.h"
#include <chrono>
#include <list>
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;

struct FrameResult {
    int npcdTests;
    int collisions;
    float bpcdTime;
    float npcdTime;
    float collisionResponseTime;
    float totalTime;
    float drawTime;
    NPCDData npcdData;
    FrameResult(int, int, float, float, float, float, float, NPCDData);
    FrameResult();
};

struct SimulationResults {
    list<FrameResult> frameResults;
    void addFrameResults(int, int, chrono::system_clock::time_point, chrono::system_clock::time_point, chrono::system_clock::time_point,
        chrono::system_clock::time_point, chrono::system_clock::time_point, chrono::system_clock::time_point, NPCDData);
    SimulationResults();
};

class LoggingManager {
private:
    static void log(std::ofstream&, FrameResult, int);
public:
    static void logRunResults(string, string, SimulationResults);
    static void logManyRunsResults(string, string, FrameResult*, int, int);
    static void logBenchmarkResults(list<SimulationResults>, Config, string);
};

#endif