#include "../include/CenteredCamera.h"

CenteredCamera::CenteredCamera(double radius, double pitch, double yaw): Camera() {
    this->radius = radius;
    this->pitch = pitch;
    this->yaw = yaw;
    this->move = false;
}

CenteredCamera::CenteredCamera(): Camera() {
    this->radius = -20.8; // sqrt(y^2 + z^2)
    this->pitch = -27;
    this->yaw = 90;
    this->move = false;
}

void CenteredCamera::lookAt() {
    double eyeX = sin(yaw * M_PI / 180) * cos(pitch * M_PI / 180) * radius;
    double eyeY = sin(pitch * M_PI / 180) * radius;
    double eyeZ = cos(yaw * M_PI / 180) * cos(pitch * M_PI / 180) * radius;
    double centerX = 0;
    double centerY = 0;
    double centerZ = 0;
    double upX = 0;
    double upY = 1;
    double upZ = 0;
    gluLookAt(eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ);
}

void CenteredCamera::eventUpdate(SDL_Event event) {
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN:
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))
            move= true;
        break;
    case SDL_MOUSEBUTTONUP:
        move= false;
        break;
    case SDL_MOUSEMOTION:
        if (move) {
            yaw -= event.motion.xrel * 0.4;
            pitch -= event.motion.yrel * 0.4;
            if (pitch > -0.5) pitch = -0.5; // Avoid camera going under the floor
            else if (pitch < -90) pitch = -90; // Avoid camera flipping when rotating to the top of the scene
        }
        break;
    case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_s:
            radius -= .1;
            break;
        case SDLK_w:
            if (radius < 0) radius += .1;
            break;
        }
    }
    }
}
