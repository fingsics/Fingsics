#ifndef SCENERECORDER_H
#define SCENERECORDER_H

#include "ConfigLoader.h"
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
constexpr uint32_t CAPSULE = 1;
constexpr uint32_t TILE = 2;

constexpr uint8_t NODRAW = 0b00000000;
constexpr uint8_t DRAW = 0b11111111;

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

struct SerializedMatrix {
    uint32_t values[9];
};

class SceneRecorder {
private:
    int numObjects;
    int frames;
    SerializedObject* objects;
    SerializedPosition** positions;
    SerializedMatrix** rotationMatrices;
    string path;
    void serializeObject(Object*, SerializedObject*);
    Object* deserializeObject(SerializedObject, SerializedPosition*, SerializedMatrix*, int, string, int);
    void serializePosition(Point, SerializedPosition*);
    void deserializePosition(SerializedPosition, Point*);
    void serializeRotationMatrix(Matrix, SerializedMatrix*);
    void deserializeRotationMatrix(SerializedMatrix, Matrix*);
public:
    SceneRecorder(Object**, int, int, string); // Recorder
    SceneRecorder(string); // Loader
    void recordFrame(Object**, int, int);
    void storeRecordedData(int);
    vector<Object*> importRecordedScene(Config);
};


#endif