#include "../include/LoggingManager.h"

FrameResult::FrameResult(int npcdTests, int collisions, float bpcdTime, float npcdTime, float collisionResponseTime, float drawTime, float totalTime) {
    this->npcdTests = npcdTests;
    this->collisions = collisions;
    this->bpcdTime = bpcdTime;
    this->npcdTime = npcdTime;
    this->collisionResponseTime = collisionResponseTime;
    this->totalTime = totalTime;
    this->drawTime = drawTime;
}

FrameResult::FrameResult() {
    this->npcdTests = 0;
    this->collisions = 0;
    this->bpcdTime = 0;
    this->npcdTime = 0;
    this->collisionResponseTime = 0;
    this->totalTime = 0;
    this->drawTime = 0;
}

SimulationResults::SimulationResults() {
    frameResults = list<FrameResult>();
}

void SimulationResults::addFrameResults(int numBroadPhaseCollisions, int numCollisions, chrono::system_clock::time_point frameStart,
										chrono::system_clock::time_point broadEnd, chrono::system_clock::time_point narrowEnd,
										chrono::system_clock::time_point responseEnd, chrono::system_clock::time_point drawStart,
                                        chrono::system_clock::time_point drawEnd) {

    float bpcdTime = (float)chrono::duration_cast<std::chrono::microseconds>(broadEnd - frameStart).count() / 1000.0;
    float npcdTime = (float)chrono::duration_cast<std::chrono::microseconds>(narrowEnd - broadEnd).count() / 1000.0;
    float collisionResponseTime = (float)chrono::duration_cast<std::chrono::microseconds>(responseEnd - narrowEnd).count() / 1000.0;
    float drawTime = (float)chrono::duration_cast<std::chrono::microseconds>(drawEnd - drawStart).count() / 1000.0;
    float totalTime = (float)chrono::duration_cast<std::chrono::microseconds>(drawEnd - frameStart).count() / 1000.0;

    FrameResult frameResult = FrameResult(numBroadPhaseCollisions, numCollisions, bpcdTime, npcdTime, collisionResponseTime, drawTime, totalTime);
    frameResults.push_back(frameResult);
}

void LoggingManager::logRunResults(string folderName, string outputFileName, SimulationResults results) {
    ofstream outputCSV;
    outputCSV.open(folderName + "\\" + outputFileName);
    outputCSV << "BPCDTime,NPCDTests,NPCDTime,Collisions,CRTime,DrawTime,TotalTime\n";
    for (auto it = results.frameResults.begin(); it != results.frameResults.end(); ++it) {
        log(outputCSV, *it, 1);
    }
    outputCSV.close();
}

void LoggingManager::logManyRunsResults(string folderName, string outputFileName, FrameResult* results, int numFrames, int numRuns) {
    ofstream outputCSV;
    outputCSV.open(folderName + "\\" + outputFileName);
    outputCSV << "BPCDTime,NPCDTests,NPCDTime,Collisions,CRTime,DrawTime,TotalTime\n";
    for (int i = 0; i < numFrames; i++) {
        log(outputCSV, results[i], numRuns);
    }
    outputCSV.close();
}

void LoggingManager::logBenchmarkResults(list<SimulationResults> results, Config config, string outputFolder) {
    if (!filesystem::is_directory("benchmarks") || !filesystem::exists("benchmarks")) filesystem::create_directory("benchmarks");
    if (filesystem::exists(outputFolder) && filesystem::is_directory(outputFolder)) filesystem::remove_all(outputFolder);
    filesystem::create_directory(outputFolder);

    float avgBpcdTime = 0.0;
    float avgNpcdTime = 0.0;
    float avgTotalTime = 0.0;
    int npcdTests = 0;
    int collisions = 0;
    int runNumber = 1;

    int numFrames = results.front().frameResults.size();
    FrameResult* avgResultsPerFrame = new FrameResult[numFrames];

    for (auto it = results.begin(); it != results.end(); ++it) {
        SimulationResults runResults = *it;
        logRunResults(outputFolder, "run" + to_string(runNumber) + ".csv", runResults);
        int frame = 0;
        for (auto it2 = runResults.frameResults.begin(); it2 != runResults.frameResults.end(); ++it2) {
            avgBpcdTime += it2->bpcdTime;
            avgNpcdTime += it2->npcdTime;
            avgTotalTime += it2->totalTime;
            npcdTests += it2->npcdTests;
            collisions += it2->collisions;

            avgResultsPerFrame[frame].bpcdTime += it2->bpcdTime;
            avgResultsPerFrame[frame].npcdTime += it2->npcdTime;
            avgResultsPerFrame[frame].totalTime += it2->totalTime;
            avgResultsPerFrame[frame].npcdTests += it2->npcdTests;
            avgResultsPerFrame[frame].collisions += it2->collisions;

            frame++;
        }

        runNumber++;
    }

    logManyRunsResults(outputFolder, "averages_per_frame.csv", avgResultsPerFrame, numFrames, results.size());

    avgBpcdTime /= config.numRuns;
    avgNpcdTime /= config.numRuns;
    avgTotalTime /= config.numRuns;
    npcdTests /= config.numRuns;
    collisions /= config.numRuns;
    
    ofstream summaryFile;
    summaryFile.open(outputFolder + "\\summary.txt");
    summaryFile << "Scene: " + config.sceneName + "\n";
    summaryFile << "FPS: " + to_string(config.fpsCap) + "\n";
    summaryFile << "BPCD Algorithm: " + config.getBPCDDescription() + "\n";
    summaryFile << "Number of runs: " + to_string(config.numRuns) + "\n";
    summaryFile << "Number of frames per run: " + to_string(config.stopAtFrame) + "\n\n";
    
    summaryFile << "Avg. BPCD time: " + to_string(avgBpcdTime) + " ms\n";
    summaryFile << "Avg. NPCD time: " + to_string(avgNpcdTime) + " ms\n";
    summaryFile << "Avg. Total time: " + to_string(avgTotalTime) + " ms\n";
    summaryFile << "NPCD tests per run: " + to_string(npcdTests) + "\n";
    summaryFile << "Collisions per run: " + to_string(collisions);

    summaryFile.close();
}

void LoggingManager::log(std::ofstream& outputFile, FrameResult frameResult, int divisor) {
    outputFile << frameResult.bpcdTime / divisor;
    outputFile << ",";
    outputFile << frameResult.npcdTests / divisor;
    outputFile << ",";
    outputFile << frameResult.npcdTime / divisor;
    outputFile << ",";
    outputFile << frameResult.collisions / divisor;
    outputFile << ",";
    outputFile << frameResult.collisionResponseTime / divisor;
    outputFile << ",";
    outputFile << frameResult.drawTime / divisor;
    outputFile << ",";
    outputFile << frameResult.totalTime / divisor;
    outputFile << "\n";
}