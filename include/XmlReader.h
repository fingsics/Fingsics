#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Color.h"
#include "Point.h"
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
	Point pos;
	Point vel;
	Point ang;
	Point angVel;
	Point acceleration;
	float mass;
	float elasticityCoef;
	Color color;
	bool isStatic;
	bool draw;
	CommonFields(Point, Point, Point, Point, Point, float, float, Color, bool, bool);
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
	Point parsePoint(const char*);
	Color parseColor(const char*);
	void checkFileExists();
public:
	XmlReader(string, int);
	vector<Object*> getObjects();
	CenteredCamera* getCamera();
};

#endif OBJLOADER_H