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
    float pitch;
    float yaw;
    Point getCenter();
public:
    FreeCamera(Point, float, float);
    FreeCamera();
    void lookAt();
    void eventUpdate(SDL_Event);
};

#endif
