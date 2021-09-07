#ifndef TILE_H
#define TILE_H
#define _USE_MATH_DEFINES

#include "SDL_opengl.h"
#include "Object.h"

using namespace std;

class Tile : public Object {
    private:
        bool draw;
        float axis1Length;
        float axis2Length;
        Point axis1;
        Point axis2;
        Point end1;
        Point end2;
        Point end3;
        Point end4;
    public:
        Tile(string, bool, Point, Point, Point, Point, Point, float, float, Color, float, float, bool);
        Tile(string, Color, Point*, Matrix*, int, float, float, bool);
        void drawObject();
        Point getNormal();
        Matrix getInertiaTensor();
        void setRotation(Matrix);
        void drawOBB();
        Point getEnd1();
        Point getEnd2();
        Point getEnd3();
        Point getEnd4();
        Point getAxis1();
        Point getAxis2();
        float getAxis1Length();
        float getAxis2Length();
        float getMinX();
        float getMinY();
        float getMinZ();
        float getMaxX();
        float getMaxY();
        float getMaxZ();
        bool getDraw();
};

#endif
