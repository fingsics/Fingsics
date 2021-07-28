#include "../include/Impulse.h"

using namespace std;

Impulse::Impulse(Point normal, Point tangent, double magnitude, double mass) {
    this->normal = normal;
    this->tangent = tangent;
    this->magnitude = magnitude;
    this->mass = mass;
}