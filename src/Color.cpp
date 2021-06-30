#include "../include/Color.h"

using namespace std;

Color::Color() {
    this->r = 255;
    this->g = 255;
    this->b = 255;
}

Color::Color(int r, int g, int b){
    if (r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255) throw "Invalid color values.";
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

Color Color::darken(double percentage) {
    double reduction = min(100.0, max(0.0, percentage)) / 100;
    return Color(r - r * reduction, g - g * reduction, b - b * reduction);
}

Color Color::lighten(double percentage) {
    double reduction = min(100.0, max(0.0, percentage)) / 100;
    return Color(r + r * reduction, g + g * reduction, b + b * reduction);
}
