#ifndef BEG_AFFINE_H
#define BEG_AFFINE_H

#include <bmath.h>

namespace BEG {

namespace Affine {
    Matrix<4> translation(Number x, Number y, Number z);
    Matrix<4> translation(Number t);
    Matrix<4> translation(const Vector<3>& t);

    Matrix<4> scale(Number x, Number y, Number z);
    Matrix<4> scale(Number s);
    Matrix<4> scale(const Vector<3>& s);

    Matrix<4> rotation(Number theta, const Vector<3>& axis);
}

}

#endif