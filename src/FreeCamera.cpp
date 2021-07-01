#include "../include/FreeCamera.h"

FreeCamera::FreeCamera(Point eye, double pitch, double yaw): Camera() {
    this->eye = eye;
    this->pitch = pitch;
    this->yaw = yaw;
}

FreeCamera::FreeCamera(): Camera() {
    this->eye = Point(-13,6,0);
    this->pitch = -25;
    this->yaw = 0;
}

Point FreeCamera::getCenter() {
    return eye + Point(1,0,0).rotate(Point(0,yaw,pitch));
}

void FreeCamera::lookAt() {
    double eyeX = eye.getX();
    double eyeY = eye.getY();
    double eyeZ = eye.getZ();
    Point center = getCenter();
    double upX = 0;
    double upY = 1;
    double upZ = 0;
    gluLookAt(eyeX,eyeY,eyeZ,center.getX(),center.getY(),center.getZ(),upX,upY,upZ);
}

void FreeCamera::eventUpdate(SDL_Event event) {
    Camera::eventUpdate(event);
    Point lookingDirection = getCenter() - eye;
    Point perpendicularLookingDirection = lookingDirection.crossProduct(Point(0,1,0)).normalize();
    switch (event.type) {
    case SDL_MOUSEMOTION:
        if (move) {
            yaw -= event.motion.xrel * 0.4;
            pitch -= event.motion.yrel * 0.4;
            if (pitch > 90) pitch = 90;
            else if (pitch < -90) pitch = -90;
        }
        break;
    case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            eye = eye + lookingDirection * 0.3;
            break;
        case SDLK_a:
            eye = eye - perpendicularLookingDirection * 0.3;
            break;
        case SDLK_s:
            eye = eye - lookingDirection * 0.3;
            break;
        case SDLK_d:
            eye = eye + perpendicularLookingDirection * 0.3;
            break;
        case SDLK_SPACE:
            eye = eye + Point(0, 1, 0) * 0.3;
            break;
        case SDLK_LSHIFT:
            eye = eye - Point(0, 1, 0) * 0.3;
            break;
        }
    }
    }
}
