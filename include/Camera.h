#ifndef CAMERA_H
#define CAMERA_H
#include <string>
#include "freeglut.h"
#include "SDL.h"
#include "SDL_opengl.h"

#define M_PI 3.1415926

using namespace std;

class Camera {
protected:
    double move;
public:
    Camera();
    virtual void lookAt() = 0;
    virtual void eventUpdate(SDL_Event) = 0;
};

#endif
