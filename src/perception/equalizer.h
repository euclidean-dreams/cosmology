#pragma once

#include "axioms.h"

namespace cosmology {

class Equalizer : public Name {
private:
    float gain;

public:
    Equalizer(float gain) : gain{gain} {

    }


    void nudge_gain(float delta) {
        gain += delta;
        if (gain < 0) gain = 0.01;
    }

    uptr<Signal<float>> equalize(uptr<Signal<float>> origin) {
        auto output = mkuptr<Signal<float>>();
        for (auto &sample: *origin) {
            output->push_back(sample * gain);
        }
        return output;
    }

    float get_gain() { return gain; }
};

}
