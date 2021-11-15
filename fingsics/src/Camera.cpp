#include "../include/Camera.h"

Camera::Camera() {
	move = false;
}

void Camera::update(SDL_Event event) {
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN:
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3))
            move = true;
        break;
    case SDL_MOUSEBUTTONUP:
        move = false;
        break;
    }
}