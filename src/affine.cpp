#include <affine.h>

using namespace BEG;

Matrix<4> Affine::translation(Number x, Number y, Number z) {
    return {
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix<4> Affine::translation(Number t) { return Affine::translation(t, t, t); }
Matrix<4> Affine::translation(const Vector<3>& t) { return Affine::translation(t.x(), t.y(), t.z()); }

Matrix<4> Affine::scale(Number x, Number y, Number z) {
    return {
        x,    0.0f, 0.0f, 0.0f,
        0.0f, y,    0.0f, 0.0f,
        0.0f, 0.0f, z,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };        
}

Matrix<4> Affine::scale(Number s) { return Affine::scale(s, s, s); }
Matrix<4> Affine::scale(const Vector<3>& s) { return Affine::scale(s.x(), s.y(), s.z()); }

Matrix<4> Affine::rotation(Number theta, const Vector<3>& axis) {
    Vector<3> n{ axis.normalized() };
    Number x{ n.x() }, y{ n.y() }, z{ n.z() };
    Number x2{ x * x }, y2{ y * y }, z2{ z * z };
    Number s{ sinf(theta) }, c{ cosf(theta) };
    return {
        c + (x2 * (1 - c)), (x * y * (1 - c)) - (z * s), (x * z * (1 - c)) + (y * s), 0.0f,
        (y * x * (1 - c)) + (z * s), c + (y2 * (1 - c)), (y * z * (1 - c)) - (x * s), 0.0f,
        (z * x * (1 - c)) - (y * s), (z * y * (1 - c)) + (x * s), c + (z2 * (1 - c)), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}