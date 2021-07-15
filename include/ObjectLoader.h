#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "Color.h"
#include "Point.h"
#include "Ball.h"
#include "Capsule.h"
#include "Plane.h"
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
	Point acceleration;
	double mass;
	double elasticityCoef;
	Color color;
	bool isStatic;
	CommonFields(Point, Point, Point, Point, Point, double, double, Color, bool);
};

class ObjectLoader {
private:
	string scene;
	int numLatLongs;
	
    CommonFields parseCommonFields(tinyxml2::XMLElement*);
	Ball* loadBall(tinyxml2::XMLElement*, string, int);
	Capsule* loadCapsule(tinyxml2::XMLElement*, string, int);
	Plane* loadPlane(tinyxml2::XMLElement*, string);
	vector<double> parseTriplet(const char* charPoint);
	Point parsePoint(const char*);
	Color parseColor(const char*);
public:
	ObjectLoader(string, int);
	vector<Object*> getObjects();
};

#endif OBJLOADER_H