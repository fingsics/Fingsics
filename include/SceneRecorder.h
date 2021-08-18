#ifndef SCENERECORDER_H
#define SCENERECORDER_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <filesystem>

using namespace std;

#define BALL 0;
#define CAPSULE 1;
#define PLANE 2;

struct SerializedPosition {
    uint32_t x;
    uint32_t y;
    uint32_t z;
};

struct SerializedRotationMatrix {
    uint32_t values[9];
};

struct SerializedObject {
    uint8_t type;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint32_t dim1;
    uint32_t dim2;
};

void recordScene();
void importScene();

#endif