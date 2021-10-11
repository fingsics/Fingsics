#ifndef SCENE_H
#define SCENE_H

#include "Object.h"
#include "Camera.h"

using namespace std;

struct Scene {
    Object** objects;
    int numObjects;
    Camera* currentCamera;
    Camera* freeCamera;
    Camera* centeredCamera;
    int stopAtFrame;
    int fpsCap;
    Scene(Object**, int, Camera*, Camera*, Camera*, int, int);
};

#endif