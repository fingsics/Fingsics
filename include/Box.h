#ifndef BOX_H
#define BOX_H
#include "SDL_opengl.h"
#include "Color.h"
#include "Point.h"
#include "Object.h"
#include <vector>
#include <math.h>
#include <string>

using namespace std;

class Box : public Object {
    private:
        // From longest to shortest
        Point dimensions;
        // Pitch, yaw, roll
        //Point torque;????
    public:
        Box(string, Point, Point, Point, Point, Point, Point, double, double, Color);
        void draw();
        Point getDimensions();
        void updatePosAndVel(double);
};

#endif
