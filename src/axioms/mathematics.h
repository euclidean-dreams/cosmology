#pragma once

#include <cmath>
#include "constants.h"
#include "macros.h"

namespace cosmology {

static inline int embind(int lower_bound, int value, int upper_bound) {
    if (value > upper_bound) value = upper_bound;
    if (value < lower_bound) value = lower_bound;
    return value;
}

static inline float embind_flt(float lower_bound, float value, float upper_bound) {
    if (value > upper_bound) value = upper_bound;
    if (value < lower_bound) value = lower_bound;
    return value;
}

static inline int cyclic_embind(int lower_bound, int value, int upper_bound) {
    if (value > upper_bound) value = lower_bound;
    if (value < lower_bound) value = upper_bound;
    return value;
}

static inline int cyclic_embind_flt(float lower_bound, float value, float upper_bound) {
    if (value > upper_bound) value = lower_bound;
    if (value < lower_bound) value = upper_bound;
    return value;
}


class Randomizer {
private:
    static int generate_number(int exclusive_max);

public:
    static int generate(int exclusive_max);

    static float generate_proportion();

    static float generate_sign();
};

}
