#include <camera.h>

using namespace BEG;

float Camera::fov() {
    return mFov;
}

void Camera::fov(float value) {
    mFov = value;
}

Vector<3> Camera::front() const {
    return { orientation.toMatrix() * Vector<4>(0.0f, 0.0f, -1.0f, 1.0f) };
}

Vector<3> Camera::right() const {
    return { orientation.toMatrix() * Vector<4>(1.0f, 0.0f, 0.0f, 1.0f) };
}

Vector<3> Camera::up() const {
    return { orientation.toMatrix() * Vector<4>(0.0f, 1.0f, 0.0f, 1.0f) };
}

Matrix<4> Camera::transformationMatrix() const {
    return Affine::translation(position) * orientation.toMatrix();
}

Matrix<4> Camera::viewMatrix() const {
    return transformationMatrix().inverse();
}

Matrix<4> Camera::perspectiveMatrix(float aspect, float near, float far) const {
    float top{ tanf(mFov * 0.5f) * near };
    float bottom{ -top };
    float right{ aspect * top };
    float left{ aspect * bottom };

    return {
        (2.0f * near) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
        0.0f, (2.0f * near) / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
        0.0f, 0.0f, -((far + near) / (far - near)), -((2.0f * far * near) / (far - near)),
        0.0f, 0.0f, -1.0f, 0.0f
    };
}

Matrix<4> Camera::combinedMatrix(float aspect, float near, float far) const {
    return perspectiveMatrix(aspect, near, far) * viewMatrix();
}