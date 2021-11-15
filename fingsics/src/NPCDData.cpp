#include "../include/NPCDData.h"

NPCDData::NPCDData(int capsuleCapsuleTests, int ballCapsuleTests, int ballBallTests, int capsuleTileTests, int ballTileTests,
				   float capsuleCapsuleTime, float ballCapsuleTime, float ballBallTime, float capsuleTileTime, float ballTileTime) {
	this->capsuleCapsuleTests = capsuleCapsuleTests;
	this->ballCapsuleTests = ballCapsuleTests;
	this->ballBallTests = ballBallTests;
	this->capsuleTileTests = capsuleTileTests;
	this->ballTileTests = ballTileTests;
	this->capsuleCapsuleTime = capsuleCapsuleTime;
	this->ballCapsuleTime = ballCapsuleTime;
	this->ballBallTime = ballBallTime;
	this->capsuleTileTime = capsuleTileTime;
	this->ballTileTime = ballTileTime;
};

NPCDData::NPCDData() {
	this->capsuleCapsuleTests = 0;
	this->ballCapsuleTests = 0;
	this->ballBallTests = 0;
	this->capsuleTileTests = 0;
	this->ballTileTests = 0;
	this->capsuleCapsuleTime = 0;
	this->ballCapsuleTime = 0;
	this->ballBallTime = 0;
	this->capsuleTileTime = 0;
	this->ballTileTime = 0;
};