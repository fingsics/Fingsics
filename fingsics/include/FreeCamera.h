#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H

#define _USE_MATH_DEFINES
#include "Camera.h"
#include "Vector.h"
#include <string>
#include <math.h>


using namespace std;

class FreeCamera: public Camera {
private:
    Vector eye;
    float pitch;
    float yaw;
    Vector getCenter();
public:
    FreeCamera(Vector, float, float);
    FreeCamera();
    void lookAt();
    void update(SDL_Event);
};

#endif
