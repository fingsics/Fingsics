#ifndef HELPERS_H
#define HELPERS_H
#include <map>
#include <string>
#include "Object.h"
#include "Matrix.h"
#include "Vector.h"

using namespace std;

Vector average(list<Vector*>);

int sign(float);

pair<string,pair<Object*, Object*>> getObjectPairWithId(Object*, Object*);

float** getMatrixForGausianElimination(Matrix, Vector);
tuple<float, float, float> solveLinearSystem(Matrix, Vector);

#endif