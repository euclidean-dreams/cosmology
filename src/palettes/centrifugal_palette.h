#pragma once

#include "axioms.h"
#include "luon.h"

namespace cosmology {

class CentrifugalPalette : public Name {
private:
    uptr<Harmony> harmony;
    SignalAverage signal_average;
    float hue;
    float flux_divisor;

public:
    CentrifugalPalette(uptr<Harmony> harmony, uint signal_average_history_length) :
            harmony{mv(harmony)},
            signal_average{signal_average_history_length},
            hue{181},
            flux_divisor{1} {

    }


    HSLColor get_color() {
        return {scint(hue), 100, 70};
    }

    void step() {
        auto harmony_energy = 0;
        for (auto &luon: *harmony->luons) {
            harmony_energy += luon->energy;
        }
        auto flux = harmony_energy - signal_average.value;
        signal_average.add_sample(harmony_energy);
        auto flux_modifier = COLOR * 10;
        if (flux_modifier < 0.01) {
            flux_modifier = 0.01;
        }
        auto hue_modifier = (flux / flux_divisor) * flux_modifier;
        hue += cyclic_embind(0, hue_modifier, HSL_HUE_MAX);
    }
};

}
