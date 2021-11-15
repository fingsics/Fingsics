#include "../include/Color.h"

Color::Color() {
    this->r = 255;
    this->g = 255;
    this->b = 255;
}

Color::Color(int r, int g, int b) {
    if (r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255) throw std::runtime_error("Invalid color values in scene");
    this->r = r;
    this->g = g;
    this->b = b;
}

int Color::getR() {
  return r;
}

int Color::getG() {
  return g;
}

int Color::getB() {
  return b;
}
