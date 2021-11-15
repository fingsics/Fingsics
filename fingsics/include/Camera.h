#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"
#include "SDL_opengl.h"
#include "freeglut.h"
#include <string>

using namespace std;

class Camera {
protected:
    float move;
public:
    Camera();
    virtual void lookAt() = 0;
    virtual void update(SDL_Event) = 0;
};

#endif
