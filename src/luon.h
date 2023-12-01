#pragma once

#include "axioms.h"

namespace cosmology {

class Luon : public Name {
public:
    float energy = 0;
    float log_energy = 0;
    int index;
    float delta = 0;
    float previous_energy = 0;

    Luon(int index) :
            index{index} {
    }

    void excite(sp<Signal<float>> &signal) {
        previous_energy = energy;
        energy = signal->get_sample(index);
        log_energy = std::log(energy + 1);
        delta = energy - previous_energy;
    }
};


class Harmony : public Name {
public:
    up<vec<sp<Luon>>> luons;

    Harmony(up<vec<sp<Luon>>> luons) : luons{mv(luons)} {

    }
};


class Psyche : public Name {
private:
    vec<sp<Luon>> luons;

public:
    Psyche(int luon_count) : luons{} {
        for (int fundamental = 0; fundamental < luon_count; fundamental++) {
            luons.push_back(mksp<Luon>(fundamental));
        }
    }

    void perceive(sp<Signal<float>> &signal) {
        for (auto &luon: luons) {
            luon->excite(signal);
        }
    }

    up<Harmony> create_harmony(vec<int> &signal_indices) {
        auto luons_in_harmony = mkup<vec<sp<Luon>>>();
        for (auto index: signal_indices) {
            luons_in_harmony->push_back(luons[index]);
        }
        return mkup<Harmony>(mv(luons_in_harmony));
    }
};

}
