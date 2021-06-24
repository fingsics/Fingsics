#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H
#define _USE_MATH_DEFINES
#include <string>
#include <math.h>
#include "Camera.h"
#include "Point.h"


using namespace std;

class FreeCamera: public Camera {
private:
    Point eye;
    double pitch;
    double yaw;
    Point getCenter();
public:
    FreeCamera(Point, double, double);
    FreeCamera();
    void lookAt();
    void eventUpdate(SDL_Event);
};

#endif
