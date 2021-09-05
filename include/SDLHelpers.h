#ifndef SDLHELPERS_H
#define SDLHELPERS_H

#include "SDL.h"
#include "SDL_opengl.h"
#include "Camera.h"
#include <iostream>

using namespace std;

void checkForInput(bool&, bool&, bool&, bool&, bool&, bool&, Camera*&, Camera*, Camera*);
SDL_Window* initializeSDL(int, int);

#endif