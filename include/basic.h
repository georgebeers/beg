#ifndef BEG_SIMPLE_H
#define BEG_SIMPLE_H

/*
 * Basic BEG components
 */

#include <bmath.h>
#include <affine.h>
#include <ecs.h>

namespace BEG {

struct Transform : Component {
    Vector<3> position{ 0.0f, 0.0f, 0.0f };
    Quaternion orientation{ 1.0f, 0.0f, 0.0f, 0.0f };
    Vector<3> scale{ 1.0f, 1.0f, 1.0f };

    Matrix<4> toMatrix() const;
};

}

#endif