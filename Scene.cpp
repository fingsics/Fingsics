#include "../include/Scene.h"

std::vector<Object*> initializeScene(double ballRad, double ballMass) {
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError eResult = xml_doc.LoadFile("settings.xml");
    tinyxml2::XMLElement* config = xml_doc.FirstChildElement("config");

    Color* ballColor = new Color(200, 200, 200);
    Color* ballColor2 = new Color(200, 20, 20);
    float height = ballRad;
    float whiteBallDistance = 1.5;
    float ballSeparation = ballRad * 1.75;
    std::vector<Object*> balls = std::vector<Object*>();

    // Parse objects
    double x, y, z, radius, mass, vx, vy, vz;
    tinyxml2::XMLElement* objects = config->FirstChildElement("objects");
    for (const tinyxml2::XMLElement* object = objects->FirstChildElement(); object; object = object->NextSiblingElement()) {
        string objectType = string(object->Name());
        if (objectType == "sphere") {
            object->QueryDoubleAttribute("x", &x);
            object->QueryDoubleAttribute("y", &y);
            object->QueryDoubleAttribute("z", &z);
            object->QueryDoubleAttribute("radius", &radius);
            object->QueryDoubleAttribute("mass", &mass);
            object->QueryDoubleAttribute("vx", &vx);
            object->QueryDoubleAttribute("vy", &vy);
            object->QueryDoubleAttribute("vz", &vz);
            balls.push_back(new Ball(std::to_string(balls.size()), x, y, z, radius, mass, balls.size() == 0 ? ballColor : ballColor2));
        }
    }

    // Set white ball initial velocity
    balls[0]->setVelocity(new Point(20, 0, 0));

    return balls;
}