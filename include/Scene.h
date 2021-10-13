#ifndef SCENE_H
#define SCENE_H

#include "Object.h"
#include "Camera.h"

using namespace std;

struct Scene {
    vector<Object*> objects;
    Camera* currentCamera;
    Camera* freeCamera;
    Camera* centeredCamera;
    int stopAtFrame;
    int fpsCap;
    Scene(vector<Object*>, Camera*, Camera*, Camera*, int, int);
};

#endif