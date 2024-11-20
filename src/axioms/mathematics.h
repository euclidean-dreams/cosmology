#pragma once

#include <cmath>
#include "constants.h"
#include "macros.h"
#include <list>

namespace cosmology {

static inline int embind(int lower_bound, int value, int upper_bound) {
    if (value > upper_bound) value = upper_bound;
    if (value < lower_bound) value = lower_bound;
    return value;
}

static inline float cyclic_embind(int lower_bound, int value, int upper_bound) {
    while (value >= upper_bound) {
        auto diff = std::abs(value - upper_bound);
        value = lower_bound + diff;
    }
    while (value < lower_bound) {
        auto diff = std::abs(value - lower_bound);
        value = upper_bound - diff;
    }
    return value;
}

static inline float embind_flt(float lower_bound, float value, float upper_bound) {
    if (value > upper_bound) value = upper_bound;
    if (value < lower_bound) value = lower_bound;
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


class SignalAverage {
private:
    lst<float> samples;
    float sum;
    size_t history_length;

public:
    float value;

    SignalAverage(size_t history_length)
            : samples{},
              sum{0},
              history_length{history_length} {

    }

    void add_sample(float sample) {
        sum += sample;
        samples.push_back(sample);

        if (samples.size() > history_length) {
            sum -= samples.front();
            samples.pop_front();
        }

        value = sum / samples.size();
    }
};

}
