#ifndef OPENGLHELPERS_H
#define OPENGLHELPERS_H

#include "Object.h"
#include <freeglut.h>
#include <fstream>

using namespace std;

void initializeOpenGL();
void setupFrame();
void drawObjects(Object**, int, bool, bool, bool);
void drawAxis();
void setLighting();
void drawFPSCounter(int);

#endif