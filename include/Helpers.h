#ifndef HELPERS_H
#define HELPERS_H
#include "Object.h"
#include "Point.h"
#include "Matrix.h"
#include <map>
#include <string>

using namespace std;

string getObjectPairId(pair<Object*, Object*> objectPair);
double** prepareForEscalerization(Matrix, Point);
double* solveLinearSystem(Matrix, Point);

#endif