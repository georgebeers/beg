#include <basic.h>

using namespace BEG;

Matrix<4> Transform::toMatrix() const {
    return Affine::translation(position) * orientation.toMatrix() * Affine::scale(scale);
}