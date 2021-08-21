#ifndef OPENGLHELPERS_H
#define OPENGLHELPERS_H

#include "Object.h"
#include <freeglut.h>

using namespace std;

void initializeOpenGL();
void setupFrame();
void drawAxis();
void setLighting();
void drawFPSCounter(int);

#endif