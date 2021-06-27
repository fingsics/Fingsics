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
                    Point(0, 0, 0),
                    Point(0, 0, 0),
                    Point(0, 0, 0),
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
        Point(0, 0, 0),
        length,
        radius,
        Point(ang1, ang2, ang3),
        Point(angv1, angv2, angv3),
        mass,
        elasticityCoef,
        Color(colorR, colorG, colorB));
}

vector<Object*> Scene::getObjects() {
    return objects;
}
