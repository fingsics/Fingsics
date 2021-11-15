#ifndef CENTERED_CAMERA_H
#define CENTERED_CAMERA_H

#define _USE_MATH_DEFINES
#include "Camera.h"
#include "Point.h"
#include <string>
#include <math.h>

using namespace std;

class CenteredCamera: public Camera {
private:
    float radius;
    float pitch;
    float yaw;

public:
    CenteredCamera(float, float, float);
    CenteredCamera();
    void lookAt();
    void update(SDL_Event);
};

#endif
