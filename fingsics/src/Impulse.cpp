#include "../include/Impulse.h"

using namespace std;

Impulse::Impulse(Vector normal, Vector tangent, float magnitude, float mass) {
    this->normal = normal;
    this->tangent = tangent;
    this->magnitude = magnitude;
    this->mass = mass;
}