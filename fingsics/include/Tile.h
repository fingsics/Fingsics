#ifndef TILE_H
#define TILE_H
#define _USE_MATH_DEFINES

#include "SDL_opengl.h"
#include "Object.h"

using namespace std;

class Tile : public Object {
    private:
        float axis1Length;
        float axis2Length;
        Vector axis1;
        Vector axis2;
        Vector end1;
        Vector end2;
        Vector end3;
        Vector end4;
    public:
        Tile(string, bool, Vector, Vector, Vector, Vector, Vector, float, float, Color, bool, float, float);
        Tile(string, Color, Vector*, Matrix*, int, bool, float, float);
        Vector getNormal();
        Matrix getInertiaTensor();
        void setRotation(Matrix);
        Vector getEnd1();
        Vector getEnd2();
        Vector getEnd3();
        Vector getEnd4();
        Vector getAxis1();
        Vector getAxis2();
        float getAxis1Length();
        float getAxis2Length();
        float getMinX();
        float getMinY();
        float getMinZ();
        float getMaxX();
        float getMaxY();
        float getMaxZ();
};

#endif
