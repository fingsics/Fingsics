#include "../include/NPCDData.h"

NPCDData::NPCDData(int capsuleCapsuleTests, int ballCapsuleTests, int ballBallTests, float capsuleCapsuleTime, float ballCapsuleTime, float ballBallTime) {
	this->capsuleCapsuleTests = capsuleCapsuleTests;
	this->ballCapsuleTests = ballCapsuleTests;
	this->ballBallTests = ballBallTests;
	this->capsuleCapsuleTime = capsuleCapsuleTime;
	this->ballCapsuleTime = ballCapsuleTime;
	this->ballBallTime = ballBallTime;
};

NPCDData::NPCDData() {
	this->capsuleCapsuleTests = 0;
	this->ballCapsuleTests = 0;
	this->ballBallTests = 0;
	this->capsuleCapsuleTime = 0;
	this->ballCapsuleTime = 0;
	this->ballBallTime = 0;
};