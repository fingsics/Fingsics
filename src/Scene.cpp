#include "../include/Scene.h"

Scene::Scene(Object** objects, int numObjects, Camera* currentCamera, Camera* freeCamera, Camera* centeredCamera, int stopAtFrame, int fpsCap) {
	this->objects = objects;
	this->numObjects = numObjects;
	this->currentCamera = currentCamera;
	this->freeCamera = freeCamera;
	this->centeredCamera = centeredCamera;
	this->stopAtFrame = stopAtFrame;
	this->fpsCap = fpsCap;
}
