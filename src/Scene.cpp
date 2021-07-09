#include "../include/Scene.h"

Scene::Scene() : Scene("scene.xml") {}

Scene::Scene(string filename) {
    string filepath= "scenes/" + filename;
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError eResult = xml_doc.LoadFile(filepath.c_str());
    tinyxml2::XMLElement* config = xml_doc.FirstChildElement("config");

    this->loadObjects(config);
}

void Scene::loadObjects(tinyxml2::XMLElement* config) {
    objects = vector<Object*>();
    double x, y, z, radius, mass, elasticityCoef, vx, vy, vz;
    int colorR, colorG, colorB;
    tinyxml2::XMLElement* xmlObjects = config->FirstChildElement("objects");
    for (tinyxml2::XMLElement* xmlObject = xmlObjects->FirstChildElement(); xmlObject; xmlObject = xmlObject->NextSiblingElement()) {
        string objectType = string(xmlObject->Name());
        if (objectType == "sphere") objects.push_back((Object*)loadBall(xmlObject, to_string(objects.size())));
        else if (objectType == "capsule") objects.push_back((Object*)loadCapsule(xmlObject, to_string(objects.size())));
    }
}

Ball* Scene::loadBall(tinyxml2::XMLElement* xmlObject, string id) {
    const char *posChar, *velChar, *angChar, *angVelChar, *forceChar, *colorChar;
    Point pos, vel, ang, angVel, force;
    Color color;
    bool isStatic;
    double radius, mass, elasticityCoef;
    tinyxml2::XMLError parseError;
    parseError = xmlObject->QueryStringAttribute("pos", &posChar);
    pos = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(posChar) : Point(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("vel", &velChar);
    vel = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(velChar) : Point(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("ang", &angChar);
    ang = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(angChar) * M_PI / 180 : Point(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("angVel", &angVelChar);
    angVel = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(angVelChar) * M_PI / 180 : Point(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("force", &forceChar);
    force = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(forceChar) : Point(0, 0, 0);
    parseError = xmlObject->QueryDoubleAttribute("mass", &mass);
    mass = (parseError == tinyxml2::XML_SUCCESS) ? mass : 1;
    parseError = xmlObject->QueryDoubleAttribute("elasticityCoef", &elasticityCoef);
    elasticityCoef = (parseError == tinyxml2::XML_SUCCESS) ? elasticityCoef : 1;
    parseError = xmlObject->QueryStringAttribute("color", &colorChar);
    color = (parseError == tinyxml2::XML_SUCCESS) ? parseColor(colorChar) : Color(200, 200, 200);
    parseError = xmlObject->QueryDoubleAttribute("radius", &radius);
    radius = (parseError == tinyxml2::XML_SUCCESS) ? radius : 1;
    parseError = xmlObject->QueryBoolAttribute("static", &isStatic);
    isStatic = (parseError == tinyxml2::XML_SUCCESS) ? isStatic : false;
    return new Ball(id, isStatic, pos, vel, ang, angVel, force, mass, elasticityCoef, color, radius);
}

Capsule* Scene::loadCapsule(tinyxml2::XMLElement* xmlObject, string id) {
    const char *posChar, *velChar, *angChar, *angVelChar, *forceChar, *colorChar;
    Point pos, vel, ang, angVel, force;
    Color color;
    bool isStatic;
    double radius, length, mass, elasticityCoef;
    tinyxml2::XMLError parseError;
    parseError = xmlObject->QueryStringAttribute("pos", &posChar);
    pos = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(posChar) : Point(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("vel", &velChar);
    vel = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(velChar) : Point(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("ang", &angChar);
    ang = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(angChar) * M_PI / 180 : Point(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("angVel", &angVelChar);
    angVel = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(angVelChar) * M_PI / 180 : Point(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("force", &forceChar);
    force = (parseError == tinyxml2::XML_SUCCESS) ? parsePoint(forceChar) : Point(0, 0, 0);
    parseError = xmlObject->QueryDoubleAttribute("mass", &mass);
    mass = (parseError == tinyxml2::XML_SUCCESS) ? mass : 1;
    parseError = xmlObject->QueryDoubleAttribute("elasticityCoef", &elasticityCoef);
    elasticityCoef = (parseError == tinyxml2::XML_SUCCESS) ? elasticityCoef : 1;
    parseError = xmlObject->QueryStringAttribute("color", &colorChar);
    color = (parseError == tinyxml2::XML_SUCCESS) ? parseColor(colorChar) : Color(200, 200, 200);
    parseError = xmlObject->QueryDoubleAttribute("radius", &radius);
    radius = (parseError == tinyxml2::XML_SUCCESS) ? radius : 1;
    parseError = xmlObject->QueryDoubleAttribute("length", &length);
    length = (parseError == tinyxml2::XML_SUCCESS) ? length : 1;
    parseError = xmlObject->QueryBoolAttribute("static", &isStatic);
    isStatic = (parseError == tinyxml2::XML_SUCCESS) ? isStatic : false;
    return new Capsule(id, isStatic, pos, vel, ang, angVel, force, mass, elasticityCoef, color, radius, length);
}

vector<Object*> Scene::getObjects() {
    return objects;
}

Point Scene::parsePoint(const char* charPoint) {
    string stringPoint = string(charPoint);
    stringPoint.erase(remove_if(stringPoint.begin(), stringPoint.end(), isspace), stringPoint.end());
    stringstream ss(stringPoint);
    vector<double> coords;
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        coords.push_back(stod(substr));
    }
    return Point(coords[0], coords[1], coords[2]);
}

Color Scene::parseColor(const char* charColor) {
    string stringColor = string(charColor);
    stringColor.erase(remove_if(stringColor.begin(), stringColor.end(), isspace), stringColor.end());
    stringstream ss(stringColor);
    vector<double> values;
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        values.push_back(stoi(substr));
    }
    return Color(values[0], values[1], values[2]);
}
