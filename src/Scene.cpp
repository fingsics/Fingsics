#include "../include/Scene.h"

Scene::Scene() : Scene("scene.xml") {}

Scene::Scene(string filename) {
    string filepath= "scenes/" + filename;
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError eResult = xml_doc.LoadFile(filepath.c_str());
    tinyxml2::XMLElement* config = xml_doc.FirstChildElement("config");

    this->loadObjects(config);
    this->loadRoom(config);
}

void Scene::loadObjects(tinyxml2::XMLElement* config) {
    objects = vector<Object*>();
    double x, y, z, radius, mass, elasticityCoef, vx, vy, vz;
    int colorR, colorG, colorB;
    tinyxml2::XMLElement* xmlObjects = config->FirstChildElement("objects");
    for (tinyxml2::XMLElement* xmlObject = xmlObjects->FirstChildElement(); xmlObject; xmlObject = xmlObject->NextSiblingElement()) {
        string objectType = string(xmlObject->Name());
        if (objectType == "sphere") objects.push_back((Object*)loadBall(xmlObject, to_string(objects.size())));
        else if (objectType == "box") objects.push_back((Object*)loadBox(xmlObject, to_string(objects.size())));
        else if (objectType == "capsule") objects.push_back((Object*)loadCapsule(xmlObject, to_string(objects.size())));
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

Ball* Scene::loadBall(tinyxml2::XMLElement* xmlObject, string id) {
    double x, y, z, radius, mass, elasticityCoef, vx, vy, vz;
    int colorR, colorG, colorB;
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
    return new Ball(id,
                    Point(x, y, z),
                    Point(vx, vy, vz),
                    Point(0, -9.8 * mass, 0),
                    radius,
                    mass,
                    elasticityCoef,
                    Color(colorR, colorG, colorB));
}

Capsule* Scene::loadCapsule(tinyxml2::XMLElement* xmlObject, string id) {
    double x, y, z, ang1, ang2, ang3, radius, length, mass, elasticityCoef, vx, vy, vz, angv1, angv2, angv3;
    int colorR, colorG, colorB;
    xmlObject->QueryDoubleAttribute("x", &x);
    xmlObject->QueryDoubleAttribute("y", &y);
    xmlObject->QueryDoubleAttribute("z", &z);
    xmlObject->QueryDoubleAttribute("ang1", &ang1);
    xmlObject->QueryDoubleAttribute("ang2", &ang2);
    xmlObject->QueryDoubleAttribute("ang3", &ang3);
    xmlObject->QueryDoubleAttribute("radius", &radius);
    xmlObject->QueryDoubleAttribute("length", &length);
    xmlObject->QueryDoubleAttribute("mass", &mass);
    xmlObject->QueryDoubleAttribute("elasticityCoef", &elasticityCoef);
    xmlObject->QueryDoubleAttribute("vx", &vx);
    xmlObject->QueryDoubleAttribute("vy", &vy);
    xmlObject->QueryDoubleAttribute("vz", &vz);
    xmlObject->QueryDoubleAttribute("angv1", &angv1);
    xmlObject->QueryDoubleAttribute("angv2", &angv2);
    xmlObject->QueryDoubleAttribute("angv3", &angv3);
    xmlObject->QueryIntAttribute("colorR", &colorR);
    xmlObject->QueryIntAttribute("colorG", &colorG);
    xmlObject->QueryIntAttribute("colorB", &colorB);
    return new Capsule(id,
        Point(x, y, z),
        Point(vx, vy, vz),
        Point(0, -9.8 * mass, 0),
        length,
        radius,
        Point(ang1, ang2, ang3),
        Point(angv1, angv2, angv3),
        mass,
        elasticityCoef,
        Color(colorR, colorG, colorB));
}

Box* Scene::loadBox(tinyxml2::XMLElement* xmlObject, string id) {
    double x, y, z, ang1, ang2, ang3, dim1, dim2, dim3, mass, elasticityCoef, vx, vy, vz, angv1, angv2, angv3;
    int colorR, colorG, colorB;
    xmlObject->QueryDoubleAttribute("x", &x);
    xmlObject->QueryDoubleAttribute("y", &y);
    xmlObject->QueryDoubleAttribute("z", &z);
    xmlObject->QueryDoubleAttribute("ang1", &ang1);
    xmlObject->QueryDoubleAttribute("ang2", &ang2);
    xmlObject->QueryDoubleAttribute("ang3", &ang3);
    xmlObject->QueryDoubleAttribute("dim1", &dim1);
    xmlObject->QueryDoubleAttribute("dim2", &dim2);
    xmlObject->QueryDoubleAttribute("dim3", &dim3);
    xmlObject->QueryDoubleAttribute("mass", &mass);
    xmlObject->QueryDoubleAttribute("elasticityCoef", &elasticityCoef);
    xmlObject->QueryDoubleAttribute("vx", &vx);
    xmlObject->QueryDoubleAttribute("vy", &vy);
    xmlObject->QueryDoubleAttribute("vz", &vz);
    xmlObject->QueryDoubleAttribute("angv1", &angv1);
    xmlObject->QueryDoubleAttribute("angv2", &angv2);
    xmlObject->QueryDoubleAttribute("angv3", &angv3);
    xmlObject->QueryIntAttribute("colorR", &colorR);
    xmlObject->QueryIntAttribute("colorG", &colorG);
    xmlObject->QueryIntAttribute("colorB", &colorB);
    vector<double> dimensions = {dim1,dim2,dim3};
    sort(dimensions.begin(), dimensions.end(), greater<double>());
    return new Box(id,
                   Point(x, y, z),
                   Point(vx, vy, vz),
                   Point(0, -9.8 * mass, 0),
                   Point(dimensions.at(0), dimensions.at(1), dimensions.at(2)),
                   Point(ang1, ang2, ang3),
                   Point(angv1, angv2, angv3),
                   mass,
                   elasticityCoef,
                   Color(colorR, colorG, colorB));
}

vector<Object*> Scene::getObjects() {
    return objects;
}

Room Scene::getRoom() {
    return room;
}