#ifndef SCENE_H
#define SCENE_H

#include "Color.h"
#include "Point.h"
#include "Ball.h"
#include "Box.h"
#include "Object.h"
#include "Room.h"
#include <string>
#include <algorithm>
#include "../tinyxml2/tinyxml2.h"

class Scene {
private:
	vector<Object*> objects;
	Room room;
	void loadRoom(tinyxml2::XMLElement*);
	void loadObjects(tinyxml2::XMLElement*);
	Ball* loadBall(tinyxml2::XMLElement*, string);
	Box* loadBox(tinyxml2::XMLElement*, string);
public:
	Scene(string);
	Scene();
	vector<Object*> getObjects();
	Room getRoom();
};

#endif SCENE_H