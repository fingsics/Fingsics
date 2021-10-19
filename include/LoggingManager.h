#ifndef SIMRESULTS_H
#define SIMRESULTS_H

#include "Config.h"
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
    FrameResult(int, int, float, float, float, float, float);
    FrameResult();
};

struct SimulationResults {
    list<FrameResult> frameResults;
    void addFrameResults(int, int, chrono::system_clock::time_point, chrono::system_clock::time_point, chrono::system_clock::time_point,
        chrono::system_clock::time_point, chrono::system_clock::time_point, chrono::system_clock::time_point);
    SimulationResults();
};

class LoggingManager {
private:
    static void log(std::ofstream&, FrameResult, int);
public:
    static void logRunResults(string, string, SimulationResults);
    static void logManyRunsResults(string, string, FrameResult*, int, int);
    static void logBenchmarkResults(list<SimulationResults>, Config);
};

#endif