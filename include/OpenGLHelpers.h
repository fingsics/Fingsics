#ifndef OPENGLHELPERS_H
#define OPENGLHELPERS_H

#include "Object.h"
#include <freeglut.h>

using namespace std;

void initializeOpenGL();
void setupFrame();
void drawObjects(Object**, int, bool);
void drawAxis();
void setLighting();


#endif