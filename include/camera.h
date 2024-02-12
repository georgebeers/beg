#ifndef BEG_CAMERA_H
#define BEG_CAMERA_H

#include <bmath.h>
#include <affine.h>

namespace BEG {

class Camera {
private:
    float mFov{};
public:
    Vector<3> position{};
    Quaternion orientation{};

    Camera(float fov = toRadians(45.0f)) : mFov{ fov }, position{ 0.0f }, orientation{ 0.0f, 0.0f, 0.0f } {}

    float fov();
    void fov(float value);

    Vector<3> front() const;
    Vector<3> right() const;
    Vector<3> up() const;

    Matrix<4> transformationMatrix() const;
    Matrix<4> viewMatrix() const;

    Matrix<4> perspectiveMatrix(float aspect, float near, float far) const;
    
    /* perspective matrix * view matrix */
    Matrix<4> combinedMatrix(float aspect /* aspect ratio of output */, float near, float far) const;
};

}

#endif