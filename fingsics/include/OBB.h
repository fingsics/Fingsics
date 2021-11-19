#ifndef OBB_H
#define OBB_H

#include "Vector.h"
#include "Matrix.h"

using namespace std;

struct OBB {
    private:
        Vector position;
        Vector halfLengths;
        Matrix normals;
    public:
        OBB();
        OBB(Vector, Vector, Matrix);
        void setRotation(Matrix);
        void setPosition(Vector);
        Vector getPosition();
        Vector getHalfLengths();
        Matrix getNormals();
};

#endif
