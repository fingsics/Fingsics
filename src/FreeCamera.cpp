#include "../include/FreeCamera.h"

FreeCamera::FreeCamera(Point eye, double pitch, double yaw): Camera() {
    this->eye = eye;
    this->pitch = pitch;
    this->yaw = yaw;
}

FreeCamera::FreeCamera(): Camera() {
    this->eye = Point(-2,1,0);
    this->pitch = 0;
    this->yaw = -90;
}

Point FreeCamera::getCenter() {
    return eye + Point(cos(yaw * M_PI / 180) * cos(pitch * M_PI / 180),
                       sin(pitch * M_PI / 180),
                       sin(yaw * M_PI / 180) * cos(pitch * M_PI / 180));
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
    Point lookingDirection = getCenter() - eye;
    Point perpendicularLookingDirection = lookingDirection.crossProduct(Point(0,1,0));
    Point perpendicularLookingDirectionNormalized = perpendicularLookingDirection / perpendicularLookingDirection.magnitude();
    switch (event.type) {
    case SDL_MOUSEMOTION:
        yaw += event.motion.xrel * 0.4;
        pitch -= event.motion.yrel * 0.4;
        if (pitch > 85) pitch = 85;
        else if (pitch < -85) pitch = -85;
        break;
    case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
        case SDLK_w:
            eye = eye + lookingDirection * 0.1;
            break;
        case SDLK_a:
            eye = eye - perpendicularLookingDirectionNormalized * 0.1;
            break;
        case SDLK_s:
            eye = eye - lookingDirection * 0.1;
            break;
        case SDLK_d:
            eye = eye + perpendicularLookingDirectionNormalized * 0.1;
            break;
        }
    }
    }
}
