#include "../include/CenteredCamera.h"

CenteredCamera::CenteredCamera(double radius, double pitch, double yaw) : Camera() {
    this->radius = radius;
    this->pitch = pitch;
    this->yaw = yaw;
}

CenteredCamera::CenteredCamera() : Camera() {
    this->radius = -25;
    this->pitch = -25;
    this->yaw = 0;
}

void CenteredCamera::lookAt() {
    Point eye = Point(1, 0, 0).rotate(Point(0,yaw,pitch)) * radius;
    double centerX = 0;
    double centerY = 0;
    double centerZ = 0;
    double upX = 0;
    double upY = 1;
    double upZ = 0;
    gluLookAt(eye.getX(),eye.getY(),eye.getZ(),centerX,centerY,centerZ,upX,upY,upZ);
}

void CenteredCamera::eventUpdate(SDL_Event event) {
    Camera::eventUpdate(event);
    switch (event.type) {
    case SDL_MOUSEMOTION:
        if (move) {
            yaw -= event.motion.xrel * 0.4;
            pitch -= event.motion.yrel * 0.4;
            // Avoid camera flipping
            if (pitch > 90) pitch = 90;
            else if (pitch < -90) pitch = -90;
        }
        break;
    case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_s:
            radius -= .7;
            break;
        case SDLK_w:
            if (radius < 0) radius += .7;
            break;
        }
    }
    }
}
