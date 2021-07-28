#include "../include/CenteredCamera.h"

CenteredCamera::CenteredCamera(float radius, float pitch, float yaw) : Camera() {
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
    float centerX = 0;
    float centerY = 0;
    float centerZ = 0;
    float upX = 0;
    float upY = 1;
    float upZ = 0;
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
            if (pitch > 89) pitch = 89;
            else if (pitch < -89) pitch = -89;
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
