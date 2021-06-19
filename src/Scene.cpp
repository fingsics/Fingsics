#include "../include/Scene.h"

Scene::Scene() : Scene("scene.xml") {}

Scene::Scene(string filename) {
    string filepath= "scenes/" + filename;
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError eResult = xml_doc.LoadFile(filepath.c_str());
    tinyxml2::XMLElement* config = xml_doc.FirstChildElement("config");

    this->loadRoom(config);
    this->loadObjects(config);
}

void Scene::loadObjects(tinyxml2::XMLElement* config) {
    objects = vector<Object*>();
    double x, y, z, radius, mass, elasticityCoef, vx, vy, vz;
    int colorR, colorG, colorB;
    tinyxml2::XMLElement* xmlObjects = config->FirstChildElement("objects");
    for (const tinyxml2::XMLElement* xmlObject = xmlObjects->FirstChildElement(); xmlObject; xmlObject = xmlObject->NextSiblingElement()) {
        string objectType = string(xmlObject->Name());
        if (objectType == "sphere") {
            xmlObject->QueryDoubleAttribute("x", &x);
            xmlObject->QueryDoubleAttribute("y", &y);
            xmlObject->QueryDoubleAttribute("z", &z);
            xmlObject->QueryDoubleAttribute("radius", &radius);
            xmlObject->QueryDoubleAttribute("mass", &mass);
            xmlObject->QueryDoubleAttribute("elasticityCoef", &elasticityCoef);
            xmlObject->QueryDoubleAttribute("vx", &vx);
            xmlObject->QueryDoubleAttribute("vy", &vy);
            xmlObject->QueryDoubleAttribute("vz", &vz);
            xmlObject->QueryIntAttribute("colorR", &colorR);
            xmlObject->QueryIntAttribute("colorG", &colorG);
            xmlObject->QueryIntAttribute("colorB", &colorB);
            objects.push_back(new Ball(to_string(objects.size()), Point(x, y, z), Point(vx, vy, vz), Point(0, -9.8 * mass, 0), radius, mass, elasticityCoef, Color(colorR, colorG, colorB)));
        }
    }
}

void Scene::loadRoom(tinyxml2::XMLElement* config) {
    tinyxml2::XMLElement* roomElement = config->FirstChildElement("room");

    float height, floor, leftWall, rightWall, backWall, frontWall;
    roomElement->QueryFloatAttribute("height", &height);
    roomElement->QueryFloatAttribute("floor", &floor);
    roomElement->QueryFloatAttribute("leftWall", &leftWall);
    roomElement->QueryFloatAttribute("rightWall", &rightWall);
    roomElement->QueryFloatAttribute("backWall", &backWall);
    roomElement->QueryFloatAttribute("frontWall", &frontWall);

    room = Room(height, floor, leftWall, rightWall, backWall, frontWall);
}

vector<Object*> Scene::getObjects() {
    return objects;
}

Room Scene::getRoom() {
    return room;
}