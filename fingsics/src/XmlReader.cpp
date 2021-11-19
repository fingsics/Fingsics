#include "../include/XmlReader.h"

CommonFields::CommonFields(Vector p, Vector v, Vector a, Vector av, Vector acc, float m, float ec, Color c, bool s, bool d) {
    pos = p;
    vel = v;
    ang = a;
    angVel = av;
    acceleration = acc;
    mass = m;
    elasticityCoef = ec;
    color = c;
    isStatic = s;
    draw = d;
}

XmlReader::XmlReader(string scene, int numLatLongs) {
    this->scene = scene;
    this->numLatLongs = numLatLongs;
}

void XmlReader::checkFileExists() {
    string filepath = "scenes/" + scene;
    if (!filesystem::exists(filepath) || !filesystem::is_regular_file(filepath)) {
        string error1 = "A scene named \'";
        string error2 = "\' was not found in the 'scenes' folder";
        throw std::runtime_error(error1 + scene + error2);
    }
}

CenteredCamera* XmlReader::getCamera() {
    checkFileExists();

    string filepath = "scenes/" + scene;
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError eResult = xml_doc.LoadFile(filepath.c_str());
    tinyxml2::XMLElement* config = xml_doc.FirstChildElement("config");
    tinyxml2::XMLElement* cameraSettings = config->FirstChildElement("camera");

    if (!cameraSettings) return NULL;

    float pitch, yaw, rad;
    tinyxml2::XMLError parseError;
    parseError = cameraSettings->QueryFloatAttribute("rad", &rad);
    rad = (parseError == tinyxml2::XML_SUCCESS) ? rad : 40;
    parseError = cameraSettings->QueryFloatAttribute("pitch", &pitch);
    pitch = (parseError == tinyxml2::XML_SUCCESS) ? pitch : -25;
    parseError = cameraSettings->QueryFloatAttribute("yaw", &yaw);
    yaw = (parseError == tinyxml2::XML_SUCCESS) ? yaw : 0;

    return new CenteredCamera(rad, pitch, yaw);
}

vector<Object*> XmlReader::getObjects() {
    checkFileExists();

    string filepath = "scenes/" + scene;
    tinyxml2::XMLDocument xml_doc;
    tinyxml2::XMLError eResult = xml_doc.LoadFile(filepath.c_str());
    tinyxml2::XMLElement* config = xml_doc.FirstChildElement("config");

    vector<Object*> objects = vector<Object*>();
    float x, y, z, radius, mass, elasticityCoef, vx, vy, vz;
    int colorR, colorG, colorB;
    tinyxml2::XMLElement* xmlObjects = config->FirstChildElement("objects");
    for (tinyxml2::XMLElement* xmlObject = xmlObjects->FirstChildElement(); xmlObject; xmlObject = xmlObject->NextSiblingElement()) {
        string objectType = string(xmlObject->Name());
        if (objectType == "sphere") objects.push_back((Object*)loadBall(xmlObject, to_string(objects.size()), numLatLongs));
        else if (objectType == "capsule") objects.push_back((Object*)loadCapsule(xmlObject, to_string(objects.size()), numLatLongs));
        else if (objectType == "tile") objects.push_back((Object*)loadTile(xmlObject, to_string(objects.size())));
    }

    objects.shrink_to_fit();
    return objects;
}

CommonFields XmlReader::parseCommonFields(tinyxml2::XMLElement* xmlObject) {
    const char* posChar, * velChar, * angChar, * angVelChar, * accelerationChar, * colorChar;
    Vector pos, vel, ang, angVel, acceleration;
    Color color;
    bool isStatic, draw;
    float radius, mass, elasticityCoef;
    tinyxml2::XMLError parseError;

    parseError = xmlObject->QueryStringAttribute("pos", &posChar);
    pos = (parseError == tinyxml2::XML_SUCCESS) ? parseVector(posChar) : Vector(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("vel", &velChar);
    vel = (parseError == tinyxml2::XML_SUCCESS) ? parseVector(velChar) : Vector(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("ang", &angChar);
    ang = (parseError == tinyxml2::XML_SUCCESS) ? parseVector(angChar) * M_PI / 180 : Vector(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("angVel", &angVelChar);
    angVel = (parseError == tinyxml2::XML_SUCCESS) ? parseVector(angVelChar) * M_PI / 180 : Vector(0, 0, 0);
    parseError = xmlObject->QueryStringAttribute("acceleration", &accelerationChar);
    acceleration = (parseError == tinyxml2::XML_SUCCESS) ? parseVector(accelerationChar) : Vector(0, 0, 0);
    parseError = xmlObject->QueryFloatAttribute("mass", &mass);
    mass = (parseError == tinyxml2::XML_SUCCESS) ? mass : 1;
    parseError = xmlObject->QueryFloatAttribute("elasticityCoef", &elasticityCoef);
    elasticityCoef = (parseError == tinyxml2::XML_SUCCESS) ? elasticityCoef : 1;
    parseError = xmlObject->QueryStringAttribute("color", &colorChar);
    color = (parseError == tinyxml2::XML_SUCCESS) ? parseColor(colorChar) : Color(200, 200, 200);
    parseError = xmlObject->QueryBoolAttribute("static", &isStatic);
    isStatic = (parseError == tinyxml2::XML_SUCCESS) ? isStatic : false;
    parseError = xmlObject->QueryBoolAttribute("draw", &draw);
    draw = (parseError == tinyxml2::XML_SUCCESS) ? draw : true;

    return CommonFields(pos, vel, ang, angVel, acceleration, mass, elasticityCoef, color, isStatic, draw);
}

Ball* XmlReader::loadBall(tinyxml2::XMLElement* xmlObject, string id, int numLatLongs) {
    CommonFields commonFields = parseCommonFields(xmlObject);
    float radius;
    tinyxml2::XMLError parseError;
    parseError = xmlObject->QueryFloatAttribute("radius", &radius);
    radius = (parseError == tinyxml2::XML_SUCCESS) ? radius : 1;
    return new Ball(id, commonFields.isStatic, commonFields.pos, commonFields.vel, commonFields.ang, commonFields.angVel, commonFields.acceleration, commonFields.mass, commonFields.elasticityCoef, commonFields.color, commonFields.draw, radius);
}

Tile* XmlReader::loadTile(tinyxml2::XMLElement* xmlObject, string id) {
    CommonFields commonFields = parseCommonFields(xmlObject);
    float length, width;
    tinyxml2::XMLError parseError;
    parseError = xmlObject->QueryFloatAttribute("length", &length);
    length = (parseError == tinyxml2::XML_SUCCESS) ? length : 30;
    parseError = xmlObject->QueryFloatAttribute("width", &width);
    width = (parseError == tinyxml2::XML_SUCCESS) ? width : 30;
    return new Tile(id, commonFields.isStatic, commonFields.pos, commonFields.vel, commonFields.ang, commonFields.angVel, commonFields.acceleration, commonFields.mass, commonFields.elasticityCoef, commonFields.color, commonFields.draw, length, width);
}

Capsule* XmlReader::loadCapsule(tinyxml2::XMLElement* xmlObject, string id, int numLatLongs) {
    CommonFields commonFields = parseCommonFields(xmlObject);
    float radius, length;
    bool rocket;
    tinyxml2::XMLError parseError;
    parseError = xmlObject->QueryFloatAttribute("radius", &radius);
    radius = (parseError == tinyxml2::XML_SUCCESS) ? radius : 1;
    parseError = xmlObject->QueryFloatAttribute("length", &length);
    length = (parseError == tinyxml2::XML_SUCCESS) ? length : 1;
    parseError = xmlObject->QueryBoolAttribute("rocket", &rocket);
    rocket = (parseError == tinyxml2::XML_SUCCESS) ? rocket : false;
    return new Capsule(id, commonFields.isStatic, commonFields.pos, commonFields.vel, commonFields.ang, commonFields.angVel, commonFields.acceleration, commonFields.mass, commonFields.elasticityCoef, commonFields.color, commonFields.draw, rocket, radius, length);
}

vector<float> XmlReader::parseTriplet(const char* input) {
    try {
        string stringInput = string(input);
        auto f = [](unsigned char const c) { return std::isspace(c); };
        stringInput.erase(remove_if(stringInput.begin(), stringInput.end(), f), stringInput.end());
        stringstream ss(stringInput);
        vector<float> values;
        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            values.push_back(stod(substr));
        }
        return values;
    }
    catch (std::exception&) {
        throw std::runtime_error("Invalid triplet value in the scene file");
    }
}

Vector XmlReader::parseVector(const char* charPoint) {
    vector<float> values = parseTriplet(charPoint);
    return Vector(values[0], values[1], values[2]);
}

Color XmlReader::parseColor(const char* charColor) {
    vector<float> values = parseTriplet(charColor);
    return Color(values[0], values[1], values[2]);
}
