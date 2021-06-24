#ifndef CENTERED_CAMERA_H
#define CENTERED_CAMERA_H
#define _USE_MATH_DEFINES
#include <string>
#include <math.h>
#include "Camera.h"
#include "Point.h"


using namespace std;

class CenteredCamera: public Camera {
private:
    double radius;
    double pitch;
    double yaw;

public:
    CenteredCamera(double, double, double);
    CenteredCamera();
    void lookAt();
    void eventUpdate(SDL_Event);
};

#endif
