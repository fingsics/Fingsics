#ifndef OBB_H
#define OBB_H

#include "Point.h"
#include "Matrix.h"

using namespace std;

struct OBB {
    private:
        Point position;
        Point halfLengths;
        Matrix normals;
        Matrix rotatedNormals;
    public:
        OBB();
        OBB(Point, Point, Matrix);
        void setRotation(Matrix);
        void setPosition(Point);
        Point getPosition();
        Point getHalfLengths();
        Matrix getRotatedNormals();
};

#endif
