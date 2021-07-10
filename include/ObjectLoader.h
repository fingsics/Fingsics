#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Color.h"
#include "Point.h"
#include "Ball.h"
#include "Capsule.h"
#include "Object.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "../tinyxml2/tinyxml2.h"

struct CommonFields {
	Point pos;
	Point vel;
	Point ang;
	Point angVel;
	Point force;
	double mass;
	double elasticityCoef;
	Color color;
	bool isStatic;
	CommonFields(Point, Point, Point, Point, Point, double, double, Color, bool);
};

class ObjectLoader {
private:
	vector<Object*> objects;
    CommonFields parseCommonFields(tinyxml2::XMLElement*);
	void loadObjects(tinyxml2::XMLElement*);
	Ball* loadBall(tinyxml2::XMLElement*, string);
	Capsule* loadCapsule(tinyxml2::XMLElement*, string);
	vector<double> parseTriplet(const char* charPoint);
	Point parsePoint(const char*);
	Color parseColor(const char*);
public:
	ObjectLoader(string);
	ObjectLoader();
	vector<Object*> getObjects();
};

#endif OBJLOADER_H