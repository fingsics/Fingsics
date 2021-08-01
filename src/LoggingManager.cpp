#include "../include/LoggingManager.h"

FrameResult::FrameResult(int mpcdTests, int npcdTests, int collisions, float bpcdTime, float mpcdTime, float npcdTime, float collisionResponseTime, float moveTime, float totalTime) {
    this->mpcdTests = mpcdTests;
    this->npcdTests = npcdTests;
    this->collisions = collisions;
    this->bpcdTime = bpcdTime;
    this->mpcdTime = mpcdTime;
    this->npcdTime = npcdTime;
    this->collisionResponseTime = collisionResponseTime;
    this->moveTime = moveTime;
    this->totalTime = totalTime;
}

SimulationResults::SimulationResults() {
    frameResults = list<FrameResult>();
}

void SimulationResults::addFrameResults(int numBroadPhaseCollisions, int numMidPhaseCollisions, int numCollisions, chrono::system_clock::time_point frameStart,
										chrono::system_clock::time_point broadEnd, chrono::system_clock::time_point midEnd, chrono::system_clock::time_point narrowEnd,
										chrono::system_clock::time_point responseEnd, chrono::system_clock::time_point moveEnd) {

    float bpcdTime = (float)chrono::duration_cast<std::chrono::microseconds>(broadEnd - frameStart).count() / 1000.0;
    float mpcdTime = (float)chrono::duration_cast<std::chrono::microseconds>(midEnd - broadEnd).count() / 1000.0;
    float npcdTime = (float)chrono::duration_cast<std::chrono::microseconds>(narrowEnd - midEnd).count() / 1000.0;
    float collisionResponseTime = (float)chrono::duration_cast<std::chrono::microseconds>(responseEnd - narrowEnd).count() / 1000.0;
    float moveTime = (float)chrono::duration_cast<std::chrono::microseconds>(moveEnd - responseEnd).count() / 1000.0;
    float totalTime = (float)chrono::duration_cast<std::chrono::microseconds>(moveEnd - frameStart).count() / 1000.0;

    FrameResult frameResult = FrameResult(numBroadPhaseCollisions, numMidPhaseCollisions, numCollisions, bpcdTime, mpcdTime, npcdTime, collisionResponseTime, moveTime, totalTime);
    frameResults.push_back(frameResult);
}

void LoggingManager::logRunResults(string folderName, string outputFileName, SimulationResults results) {
    ofstream outputCSV;
    outputCSV.open(folderName + "\\" + outputFileName);
    outputCSV << "BPCDTime,MPCDTests,MPCDTime,NPCDTests,NPCDTime,Collisions,CRTime,MoveTime,TotalTime\n";
    for (auto it = results.frameResults.begin(); it != results.frameResults.end(); ++it) {
        log(outputCSV, *it);
    }
    outputCSV.close();
}

void LoggingManager::logBenchmarkResults(list<SimulationResults> results, Config config) {
    string outputFolder = "benchmarks\\" + config.sceneName + "_" + config.getBPCDDescription() + "_" + config.getMPCDDescription();
    
    if (!filesystem::is_directory("benchmarks") || !filesystem::exists("benchmarks")) filesystem::create_directory("benchmarks");
    if (filesystem::exists(outputFolder) && filesystem::is_directory(outputFolder)) filesystem::remove_all(outputFolder);
    filesystem::create_directory(outputFolder);

    float avgBpcdTime = 0.0;
    float avgMpcdTime = 0.0;
    float avgNpcdTime = 0.0;
    float avgTotalTime = 0.0;
    int mpcdTests = 0;
    int npcdTests = 0;
    int collisions = 0;
    int runNumber = 1;

    for (auto it = results.begin(); it != results.end(); ++it) {
        SimulationResults runResults = *it;
        logRunResults(outputFolder, "run" + to_string(runNumber) + ".csv", runResults);

        for (auto it2 = runResults.frameResults.begin(); it2 != runResults.frameResults.end(); ++it2) {
            avgBpcdTime += it2->bpcdTime;
            avgMpcdTime += it2->mpcdTime;
            avgNpcdTime += it2->npcdTime;
            avgTotalTime += it2->totalTime;
            mpcdTests += it2->mpcdTests;
            npcdTests += it2->npcdTests;
            collisions += it2->collisions;
        }

        runNumber++;
    }

    avgBpcdTime /= config.numRuns;
    avgMpcdTime /= config.numRuns;
    avgNpcdTime /= config.numRuns;
    avgTotalTime /= config.numRuns;
    mpcdTests /= config.numRuns;
    npcdTests /= config.numRuns;
    collisions /= config.numRuns;
    
    ofstream summaryFile;
    summaryFile.open(outputFolder + "\\summary.txt");
    summaryFile << "Scene: " + config.sceneName + "\n";
    summaryFile << "FPS: " + to_string(config.fps) + "\n";
    summaryFile << "BPCD Algorithm: " + config.getBPCDDescription() + "\n";
    summaryFile << "MPCD Algorithm: " + config.getMPCDDescription() + "\n";
    summaryFile << "Number of runs: " + to_string(config.numRuns) + "\n";
    summaryFile << "Number of frames per run: " + to_string(config.numFramesPerRun) + "\n\n";
    
    summaryFile << "Avg. BPCD time: " + to_string(avgBpcdTime) + " ms\n";
    summaryFile << "Avg. MPCD time: " + to_string(avgMpcdTime) + " ms\n";
    summaryFile << "Avg. NPCD time: " + to_string(avgNpcdTime) + " ms\n";
    summaryFile << "Avg. Total time: " + to_string(avgTotalTime) + " ms\n";
    summaryFile << "MPCD tests per run: " + to_string(mpcdTests) + "\n";
    summaryFile << "NPCD tests per run: " + to_string(npcdTests) + "\n";
    summaryFile << "Collisions per run: " + to_string(collisions);

    summaryFile.close();
}

void LoggingManager::log(std::ofstream& outputFile, FrameResult frameResult) {
    outputFile << frameResult.bpcdTime;
    outputFile << ",";
    outputFile << frameResult.mpcdTests;
    outputFile << ",";
    outputFile << frameResult.mpcdTime;
    outputFile << ",";
    outputFile << frameResult.npcdTests;
    outputFile << ",";
    outputFile << frameResult.npcdTime;
    outputFile << ",";
    outputFile << frameResult.collisions;
    outputFile << ",";
    outputFile << frameResult.collisionResponseTime;
    outputFile << ",";
    outputFile << frameResult.moveTime;
    outputFile << ",";
    outputFile << frameResult.totalTime;
    outputFile << "\n";
}