#ifndef NPCDDATA_H
#define NPCDDATA_H

using namespace std;

struct NPCDData {
    int capsuleCapsuleTests;
    int ballCapsuleTests;
    int ballBallTests;
    float capsuleCapsuleTime;
    float ballCapsuleTime;
    float ballBallTime;
    NPCDData(int, int, int, float, float, float);
    NPCDData();
};

#endif