#ifndef SCENERECORDER_H
#define SCENERECORDER_H

#include "Config.h"
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
constexpr uint32_t ROCKET = 3;

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
    SerializedObject* objects;
    SerializedPosition** positions;
    SerializedMatrix** rotationMatrices;
    string path;
    void serializeObject(Object*, SerializedObject*);
    Object* deserializeObject(SerializedObject, SerializedPosition*, SerializedMatrix*, int, string, int);
    void serializePosition(Vector, SerializedPosition*);
    void deserializePosition(SerializedPosition, Vector*);
    void serializeRotationMatrix(Matrix, SerializedMatrix*);
    void deserializeRotationMatrix(SerializedMatrix, Matrix*);
public:
    SceneRecorder(vector<Object*>, int, string); // Recorder
    SceneRecorder(string); // Loader
    void recordFrame(vector<Object*>, int);
    void storeRecordedData(int, int);
    tuple<vector<Object*>, int, int> importRecordedScene(Config);
};


#endif