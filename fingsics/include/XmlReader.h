#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Color.h"
#include "Vector.h"
#include "Ball.h"
#include "Capsule.h"
#include "Tile.h"
#include "Object.h"
#include "Camera.h"
#include "FreeCamera.h"
#include "CenteredCamera.h"
#include "tinyxml2.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

struct CommonFields {
	Vector pos;
	Vector vel;
	Vector ang;
	Vector angVel;
	Vector acceleration;
	float mass;
	float elasticityCoef;
	Color color;
	bool isStatic;
	bool draw;
	CommonFields(Vector, Vector, Vector, Vector, Vector, float, float, Color, bool, bool);
};

class XmlReader {
private:
	string scene;
	int numLatLongs;
	
    CommonFields parseCommonFields(tinyxml2::XMLElement*);
	Ball* loadBall(tinyxml2::XMLElement*, string, int);
	Tile* loadTile(tinyxml2::XMLElement*, string);
	Capsule* loadCapsule(tinyxml2::XMLElement*, string, int);
	vector<float> parseTriplet(const char* charPoint);
	Vector parseVector(const char*);
	Color parseColor(const char*);
	void checkFileExists();
public:
	XmlReader(string, int);
	vector<Object*> getObjects();
	CenteredCamera* getCamera();
};

#endif OBJLOADER_H