#ifndef COLOR_H
#define COLOR_H

#include <algorithm>
#include <stdexcept>

using namespace std;

class Color {
    private:
        int r;
        int g;
        int b;
    public:
        Color();
        Color(int, int, int);
        int getR();
        int getG();
        int getB();
};

#endif
