#ifndef HELPERS_H
#define HELPERS_H
#include <map>
#include <string>
#include "Object.h"
#include "Matrix.h"
#include "Point.h"

using namespace std;

string getObjectPairId(pair<Object*, Object*> objectPair);
double** prepareForEscalerization(Matrix, Point);
double* solveLinearSystem(Matrix, Point);

#endif