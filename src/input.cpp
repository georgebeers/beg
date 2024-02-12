#include <input.h>

using namespace BEG;

int Keyboard::toGLFWKey(Keyboard::Key key) {
    return static_cast<int>(key);
}

Keyboard::Key Keyboard::fromGLFWKey(int key) {
    return static_cast<Keyboard::Key>(key);
}

void Keyboard::setKeyDown(Keyboard::Key key) {
    mMap[static_cast<size_t>(key)] = true;
}

void Keyboard::setKeyUp(Keyboard::Key key) {
    mMap[static_cast<size_t>(key)] = false;
}

bool Keyboard::isDown(Keyboard::Key key) const {
    return mMap[static_cast<size_t>(key)];
}

bool Keyboard::isUp(Keyboard::Key key) const {
    return !mMap[static_cast<size_t>(key)];
}

int Mouse::toGLFWButton(Mouse::Button button) {
    return static_cast<int>(button);
}

Mouse::Button Mouse::fromGLFWButton(int button) {
    return static_cast<Mouse::Button>(button);
}

void Mouse::setButtonDown(Mouse::Button button) { 
    mMap[static_cast<size_t>(button)] = true;
}

void Mouse::setButtonUp(Mouse::Button button) { 
    mMap[static_cast<size_t>(button)] = false;
}

bool Mouse::isDown(Mouse::Button button) const {
    return mMap[static_cast<size_t>(button)];
}

bool Mouse::isUp(Mouse::Button button) const {
    return !mMap[static_cast<size_t>(button)];
}

void Mouse::setMousePosition(float x, float y) {
    mDX = x - mX;
    mDY = x - mY;

    mX = x;
    mY = y;
}

float Mouse::x() const { return mX; }
float Mouse::y() const { return mY; }
float Mouse::changeInX() const { return mDX; }
float Mouse::changeInY() const { return mDY; }