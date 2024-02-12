#ifndef BEG_COLOR_H
#define BEG_COLOR_H

#include <bmath.h>

namespace BEG {

class Color {
private:
    float mR{}, mG{}, mB{};
public:
    Color() : mR{ 0.0f }, mG{ 0.0f }, mB{ 0.0f } {}
    Color(float r, float g, float b) : mR{ r }, mG{ g }, mB{ b } {}

    float r() const;
    void r(float r);
    float g() const;
    void g(float g);
    float b() const;
    void b(float b);

    Vector<3> toVector() const;
};

}

#endif