#ifndef OPENGLHELPERS_H
#define OPENGLHELPERS_H

#include "Object.h"
#include <freeglut.h>
#include <fstream>

using namespace std;

void initializeOpenGL(int, int);
void setupFrame();
void drawAxis();
void setLighting();
void drawFPSCounter(int);

#endif