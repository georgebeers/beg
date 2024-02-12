#include <bmath.h>

using namespace BEG;

Number BEG::toRadians(Number degrees) {
    return Pi * (degrees / 180.0f);
}

Number BEG::toDegrees(Number radians) {
    return 180.0f * (radians / Pi);
}

Number BEG::clamp(Number value, Number min, Number max) {
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}