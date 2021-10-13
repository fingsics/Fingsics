#include "../include/Scene.h"

Scene::Scene(vector<Object*> objects, Camera* currentCamera, Camera* freeCamera, Camera* centeredCamera, int stopAtFrame, int fpsCap) {
	this->objects = objects;
	this->currentCamera = currentCamera;
	this->freeCamera = freeCamera;
	this->centeredCamera = centeredCamera;
	this->stopAtFrame = stopAtFrame;
	this->fpsCap = fpsCap;
}
