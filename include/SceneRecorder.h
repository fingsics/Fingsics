#ifndef SCENERECORDER_H
#define SCENERECORDER_H

#include "Object.h"
#include "Ball.h"
#include "Capsule.h"
#include "Tile.h"
#include "Color.h"
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

using namespace std;

constexpr uint32_t BALL = 0;
constexpr uint32_t CAPSULE = 0;
constexpr uint32_t TILE = 0;


struct SerializedObject {
    uint8_t type;
    uint8_t draw;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint32_t dim1;
    uint32_t dim2;
};

struct SerializedPosition {
    uint32_t x;
    uint32_t y;
    uint32_t z;
};

struct SerializedRotationMatrix {
    uint32_t values[9];
};

class SceneRecorder {
private:
    int numObjects;
    int frames;
    int currentFrame;
    SerializedObject* objects;
    SerializedPosition** positions;
    SerializedRotationMatrix** rotationMatrices;
    string path;
    void serializeObject(Object*, SerializedObject*);
    Object* deserializeObject(SerializedObject);
public:
    SceneRecorder(Object**, int, int, string);
    void recordFrame();
    void storeRecordedData();
    vector<Object*> importRecordedScene();
};


#endif