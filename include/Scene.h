#ifndef SCENE_H
#define SCENE_H

#include "Color.h"
#include "Point.h"
#include "Ball.h"
#include "Capsule.h"
#include "Object.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "../tinyxml2/tinyxml2.h"

class Scene {
private:
	vector<Object*> objects;
	void loadRoom(tinyxml2::XMLElement*);
	void loadObjects(tinyxml2::XMLElement*);
	Ball* loadBall(tinyxml2::XMLElement*, string);
	Capsule* loadCapsule(tinyxml2::XMLElement*, string);
public:
	Scene(string);
	Scene();
	vector<Object*> getObjects();
};

#endif SCENE_H