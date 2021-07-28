#ifndef HELPERS_H
#define HELPERS_H
#include <map>
#include <string>
#include "Object.h"
#include "Matrix.h"
#include "Point.h"

using namespace std;

pair<string,pair<Object*, Object*>> getObjectPairWithId(Object*, Object*);

float** getMatrixForGausianElimination(Matrix, Point);
tuple<float, float, float> solveLinearSystem(Matrix, Point);

#endif