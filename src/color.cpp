#include <color.h>

using namespace BEG;

float Color::r() const { return mR; }
void Color::r(float r) { mR = r; }

float Color::g() const { return mG; }
void Color::g(float g) { mG = g; }

float Color::b() const { return mB; }
void Color::b(float b) { mB = b; }

Vector<3> Color::toVector() const {
    return { mR, mG, mB };
}