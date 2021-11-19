#include "../include/FreeCamera.h"

FreeCamera::FreeCamera(Vector eye, float pitch, float yaw): Camera() {
    this->eye = eye;
    this->pitch = pitch;
    this->yaw = yaw;
}

FreeCamera::FreeCamera(): Camera() {
    this->eye = Vector(-13,6,0);
    this->pitch = -25;
    this->yaw = 0;
}

Vector FreeCamera::getCenter() {
    return eye + Vector(1,0,0).rotate(Vector(0,yaw,pitch));
}

void FreeCamera::lookAt() {
    float eyeX = eye.getX();
    float eyeY = eye.getY();
    float eyeZ = eye.getZ();
    Vector center = getCenter();
    float upX = 0;
    float upY = 1;
    float upZ = 0;
    gluLookAt(eyeX,eyeY,eyeZ,center.getX(),center.getY(),center.getZ(),upX,upY,upZ);
}

void FreeCamera::update(SDL_Event event) {
    Camera::update(event);
    Vector lookingDirection = getCenter() - eye;
    Vector perpendicularLookingDirection = lookingDirection.crossProduct(Vector(0,1,0)).normalize();
    switch (event.type) {
    case SDL_MOUSEMOTION:
        if (move) {
            float speed = 0.4;
            yaw -= event.motion.xrel * speed;
            pitch -= event.motion.yrel * speed;
            if (pitch > 89) pitch = 89;
            else if (pitch < -89) pitch = -89;
        }
        break;
    case SDL_KEYDOWN: {
        float speed = 0.3;
        switch (event.key.keysym.sym) {
        case SDLK_w:
            eye = eye + lookingDirection * speed;
            break;
        case SDLK_a:
            eye = eye - perpendicularLookingDirection * speed;
            break;
        case SDLK_s:
            eye = eye - lookingDirection * speed;
            break;
        case SDLK_d:
            eye = eye + perpendicularLookingDirection * speed;
            break;
        case SDLK_LCTRL:
            eye = eye + Vector(0, 1, 0) * speed;
            break;
        case SDLK_LSHIFT:
            eye = eye - Vector(0, 1, 0) * speed;
            break;
        }
    }
    }
}
