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
            float speed = 0.4;
            yaw -= event.motion.xrel * speed;
            pitch -= event.motion.yrel * speed;
            // Avoid camera flipping
            if (pitch > 90) pitch = 90;
            else if (pitch < -90) pitch = -90;
        }
        break;
    case SDL_KEYDOWN: {
        float speed = 0.7;
        switch (event.key.keysym.sym) {
        case SDLK_s:
            radius -= speed;
            break;
        case SDLK_w:
            if (radius < 0) radius += speed;
            break;
        }
    }
    }
}
