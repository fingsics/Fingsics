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

    int capsuleTileTests;
    int ballTileTests;
    float capsuleTileTime;
    float ballTileTime;

    NPCDData(int, int, int, int, int, float, float, float, float, float);
    NPCDData();
};

#endif