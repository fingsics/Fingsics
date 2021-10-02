#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "Object.h"
#include "Capsule.h"
#include "Ball.h"
#include "Tile.h"
#include <freeglut.h>
#include <fstream>

using namespace std;

class SceneRenderer {
private:
    void drawAABB(Object*);
    void drawOBB(Object*);
    void drawBall(Ball*);
    void drawCapsule(Capsule*);
    void drawTile(Tile*);
    void drawF(float, float, float, float, float);
    void drawP(float, float, float, float, float);
    void drawS(float, float, float, float, float);
    void draw0(float, float, float, float, float);
    void draw1(float, float, float, float, float);
    void draw2(float, float, float, float, float);
    void draw3(float, float, float, float, float);
    void draw4(float, float, float, float, float);
    void draw5(float, float, float, float, float);
    void draw6(float, float, float, float, float);
    void draw7(float, float, float, float, float);
    void draw8(float, float, float, float, float);
    void draw9(float, float, float, float, float);
public:
	void initializeOpenGL(int, int);
	void setupFrame();
	void drawAxis();
    void drawObject(Object*, bool, bool);
	void setLighting();
	void drawFPSCounter(int);
    void drawCharacter(char, float, float, float, float, float);
};


#endif