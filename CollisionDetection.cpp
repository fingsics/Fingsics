// CollisionDetection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "SDL.h"
#include "SDL_opengl.h"
#include "include/Scene.h"
#include "include/CenteredCamera.h"
#include "include/FreeCamera.h"
#include "include/BroadPhaseAlgorithms.h"
#include "include/MidPhaseAlgorithms.h"
#include "include/NarrowPhaseAlgorithms.h"
#include "include/Matrix.h"
#include "freeglut.h"
#include <iostream>
#include <thread>
#include <string>
#include <map>

#define _USE_MATH_DEFINES
#define FPS 60

using namespace std;

void moveObjects(Object** objects, int numObjects, float frames, bool slowMotion) {
    float time = slowMotion ? frames / 3 : frames;
    for (int i = 0; i < numObjects; i++) objects[i]->updatePosAndVel(time);
}

// Inertia values: https://www.thoughtco.com/moment-of-inertia-formulas-2698806
void applyCollisions2(map<string, tuple<Object*, Object*, Point, Point>> oldCollisions, map<string, tuple<Object*, Object*, Point, Point>> collisionMap) {
    for (auto const& mapEntry : collisionMap) {
        if (oldCollisions.find(mapEntry.first) != oldCollisions.end()) continue;
        Object* object1 = get<0>(mapEntry.second);
        Object* object2 = get<1>(mapEntry.second);
        Point collisionPoint = get<2>(mapEntry.second);
        Point collisionNormal = get<3>(mapEntry.second);

        double e = 1; // e coefficient of restitution which depends on the nature of the two colliding materials  ------>   (object1->getElasticity() + object2->getElasticity()) / 2
        double ma = object1->getMass(); // ma total mass of body a
        double mb = object2->getMass(); // mb total mass of body b
        Matrix Ia = object1->getInertiaTensor(); // Ia inertia tensor for body a in absolute coordinates
        Matrix Ib = object2->getInertiaTensor(); // Ib inertia tensor for body a in absolute coordinates
        Point ra = collisionPoint - object1->getPos(); // ra position of collision point relative to centre of mass of body a in absolute coordinates(if this is known in local body coordinates it must be converted before this is called).
        Point rb = collisionPoint - object2->getPos(); // rb position of collision point relative to centre of mass of body b in absolute coordinates(if this is known in local body coordinates it must be converted before this is called).
        Point collisionToCenterOfMass = object2->getPos() - collisionPoint;
        Point normal = collisionNormal; // n normal to collision point, the line along which the impulse acts.
        Point vai = object1->getVel(); // vai initial velocity of centre of mass on object a
        Point vbi = object2->getVel(); // vbi initial velocity of centre of mass on object b
        Point wai = object1->getAngularVelocity(); // wai initial angular velocity of object a
        Point wbi = object2->getAngularVelocity(); // wbi initial angular velocity of object b

        Point vaf; // vaf final velocity of centre of mass on object a
        Point vbf; // vbf final velocity of centre of mass on object a
        Point waf; // waf final angular velocity of object a
        Point wbf; // wbf final angular velocity of object b



        Matrix IaInverse = Ia.inverse();
        Matrix IbInverse = Ib.inverse();

        Point angularVelChangea = normal.crossProduct(ra);
        angularVelChangea = IaInverse * angularVelChangea;
        Point vaLinDueToR = angularVelChangea.crossProduct(ra);  // calculate the linear velocity of collision point on a due to rotation of a
        double scalar = 1 / ma + vaLinDueToR.dotProduct(normal);

        Point angularVelChangeb = normal.crossProduct(rb);
        angularVelChangeb = IbInverse * angularVelChangeb;
        Point vbLinDueToR = angularVelChangeb.crossProduct(rb);  // calculate the linear velocity of collision point on b due to rotation of b
        scalar += 1 / mb + vbLinDueToR.dotProduct(normal);

        double Jmod = (e + 1) * (vai - vbi).magnitude() / scalar;

        // Puede estar mal, lo hice yo
        Point collisionPlusNormal = collisionPoint + (normal * 0.01);
        double distanceToCollision = object1->getPos().distanceTo(collisionPoint);
        double distanceToCollisionPlusNormal = object1->getPos().distanceTo(collisionPlusNormal);
        bool isNormalPointingTowardsA = distanceToCollisionPlusNormal < distanceToCollision;

        Point Ja = isNormalPointingTowardsA ? normal * Jmod : normal * -Jmod;
        Point Jb = isNormalPointingTowardsA ? normal * -Jmod : normal * Jmod;

        vaf = vai + (Ja * (1 / ma));
        vbf = vbi + (Jb * (1 / mb));

        waf = wai - angularVelChangea;
        wbf = wbi - angularVelChangeb;

        object1->setVel(vaf);
        object2->setVel(vbf);
        object1->setAngularVelocity(waf);
        object2->setAngularVelocity(wbf);
    }
}


void applyCollisions(map<string, pair<Object*, Object*>> oldCollisions, map<string, pair<Object*, Object*>> collisionMap) {
    for (auto const& mapEntry : collisionMap) {
        if (oldCollisions.find(mapEntry.first) != oldCollisions.end()) continue;
        pair<Object*, Object*> collisionPair = mapEntry.second;
        Object* object1 = collisionPair.first;
        Object* object2 = collisionPair.second;
        double object1Mass = object1->getMass();
        double object2Mass = object2->getMass();
        Point object1Vel = object1->getVel();
        Point object2Vel = object2->getVel();
        Point object1Pos = object1->getPos();
        Point object2Pos = object2->getPos();

        Point normalVector = object1Pos - object2Pos;
        double distance = normalVector.magnitude();

        Point unitVector = normalVector / distance;
        Point tangentVector = Point(-unitVector.getZ(), -unitVector.getY(), unitVector.getX());

        double vector1NormalMagnitude = unitVector.dotProduct(object1Vel);
        double vector1TangentMagnitude = tangentVector.dotProduct(object1Vel);
        double vector2NormalMagnitude = unitVector.dotProduct(object2Vel);
        double vector2TangentMagnitude = tangentVector.dotProduct(object2Vel);

        // Because of conservation of kinetic energy
        double newVector1NormalMagnitude = (vector1NormalMagnitude * (object1Mass - object2Mass) + 2 * object2Mass * vector2NormalMagnitude) / (object1Mass + object2Mass);
        double newVector2NormalMagnitude = (vector2NormalMagnitude * (object2Mass - object1Mass) + 2 * object1Mass * vector1NormalMagnitude) / (object1Mass + object2Mass);

        // These are the same
        double newVector1TangentMagnitude = vector1TangentMagnitude;
        double newVector2TangentMagnitude = vector2TangentMagnitude;

        Point newVector1Normal = unitVector * newVector1NormalMagnitude;
        Point newVector1Tangent = tangentVector * newVector1TangentMagnitude;
        Point newVector2Normal = unitVector * newVector2NormalMagnitude;
        Point newVector2Tangent = tangentVector * newVector2TangentMagnitude;

        Point newVector1Velocity = (newVector1Normal + newVector1Tangent) * object1->getElasticity();
        Point newVector2Velocity = (newVector2Normal + newVector2Tangent) * object2->getElasticity();

        object1->setVel(newVector1Velocity);
        object2->setVel(newVector2Velocity);
    }
}

void drawObjects(Object** objects, int numObjects) {
    int i = 0;
    for (int i = 0; i < numObjects; i++) {
        objects[i]->draw();
    }
}

SDL_Window* initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        exit(1);
    }

    atexit(SDL_Quit);

    SDL_Window* window = SDL_CreateWindow("Fingsycs", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        cerr << "Failed to initialize view mode: " << SDL_GetError() << endl;
        exit(1);
    }
    SDL_GL_CreateContext(window);

    return window;
}

void setLighting() {
    float lightR = 1.0f;
    float lightG = 1.0f;
    float lightB = 1.0f;

    GLfloat position[] = { 0, 5, 0, 1 };
    GLfloat ambient[] = { lightR / 10, lightG / 10, lightB / 10, 0.4f };
    GLfloat specular[] = { lightR, lightG, lightB, 2.0f };
    GLfloat diffuse[] = { lightR, lightG, lightB, 2.0f };
    GLfloat direction[] = { 0, -1, 0 };

    GLfloat materialAmbient[] = { .2, .2, .2, 1 };
    GLfloat materialDiffuse[] = { .8, .8, .8, 1 };

    glPushMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);

    glPopMatrix();
}

void checkForInput(bool &slowMotion, bool &pause, bool &quit, bool& draw, Camera* &camera, Camera* freeCamera, Camera* centeredCamera) {
    SDL_Event event;
    int xm, ym;
    SDL_GetMouseState(&xm, &ym);
    while (SDL_PollEvent(&event)) {
        camera->eventUpdate(event);
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYUP: {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_q:
                quit = true;
                break;
            case SDLK_p:
                pause = !pause;
                break;
            case SDLK_m:
                slowMotion = !slowMotion;
                break;
            case SDLK_f:
                draw = !draw;
                break;
            case SDLK_c:
                if (camera == centeredCamera) camera = freeCamera;
                else camera = centeredCamera;
                break;
            default:
                break;
            }
        }
        default:
            break;
        }
    }
}

void manageFrameTime(clock_t &lastFrameTime, float &secondsSinceLastFrame) {
    double minFrameTime = 1.0 / FPS;
    secondsSinceLastFrame = (double)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    if (secondsSinceLastFrame < minFrameTime) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)((minFrameTime - secondsSinceLastFrame) * 1000)));
        secondsSinceLastFrame = (double)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
    }
    lastFrameTime = clock();
}

int main(int argc, char* argv[]) {
    SDL_Window* window = initializeSDL();

    // Camera
    Camera* centeredCamera = new CenteredCamera();
    Camera* freeCamera = new FreeCamera();
    Camera* camera = freeCamera;

    // Program options
    bool quit = false;
    bool pause = true;
    bool draw = true;
    bool slowMotion = false;
    bool showMenu = false;

    // FPS management
    clock_t lastFrameTime = clock();
    float timeSinceLastFrame = 0;

    // Collision detection algorithm
    BroadPhaseAlgorithm* broadPhaseAlgorithm = new NoBroadPhase();
    MidPhaseAlgorithm* midPhaseAlgorithm = new NoMidPhase();
    map<string, tuple<Object*, Object*, Point, Point>> oldCollisions;

    // Scene
    string sceneName = "2d-scene.xml";
    Scene scene = Scene(sceneName);
    vector<Object*> objectsVector = scene.getObjects();
    Object** objects = &objectsVector[0];
    int numObjects = objectsVector.size();

    glMatrixMode(GL_PROJECTION);
    glClearColor(0, 0, 0, 1);
    gluPerspective(45, 1280 / 720.f, 0.1, 100);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    while (!quit) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set camera position
        camera->lookAt();

        // Set light
        setLighting();

        // Draw objects
        if (draw) {
            drawObjects(objects, numObjects);
        }

        // Apply physics and movement
        if (!pause) {
            map<string, pair<Object*, Object*>> broadPhaseCollisions = broadPhaseAlgorithm->getCollisions(objects, numObjects);
            map<string, pair<Object*, Object*>> midPhaseCollisions = midPhaseAlgorithm->getCollisions(broadPhaseCollisions);
            map<string, tuple<Object*, Object*, Point, Point>> collisions = NarrowPhaseAlgorithms::getCollisions(midPhaseCollisions);
            applyCollisions2(oldCollisions, collisions);
            oldCollisions = collisions;
            moveObjects(objects, numObjects, timeSinceLastFrame, slowMotion);
        }

        // Process events
        checkForInput(slowMotion, pause, quit, draw, camera, freeCamera, centeredCamera);

        // Force FPS cap
        manageFrameTime(lastFrameTime, timeSinceLastFrame);

        SDL_GL_SwapWindow(window);
    }
    return 0;
}
