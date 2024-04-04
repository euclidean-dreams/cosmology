#pragma once

#include "axioms.h"

namespace cosmology {

class Luon : public Name {
public:
    float energy = 0;
    float log_energy = 0;
    int index;
    float delta = 0;
    float log_delta = 0;
    float previous_energy = 0;

    Luon(int index) :
            index{index} {
    }

    void excite(sptr<Signal<float>> &signal) {
        previous_energy = energy;
        energy = signal->get_sample(index);
        log_energy = std::log(energy + 1);
        delta = energy - previous_energy;
        if (delta > 0) {
            log_delta = std::log(delta + 1);
        } else {
            log_delta = -std::log(1 - delta);
        }
    }
};


class Harmony : public Name {
public:
    uptr<vec<sptr<Luon>>> luons;

    Harmony(uptr<vec<sptr<Luon>>> luons) : luons{mv(luons)} {

    }
};


class Psyche : public Name {
private:
    vec<sptr<Luon>> luons;

public:
    Psyche(int luon_count) : luons{} {
        for (int fundamental = 0; fundamental < luon_count; fundamental++) {
            luons.push_back(mksptr<Luon>(fundamental));
        }
    }

    void perceive(sptr<Signal<float>> &signal) {
        for (auto &luon: luons) {
            luon->excite(signal);
        }
    }

    uptr<Harmony> create_harmony(vec<int> &signal_indices) {
        auto luons_in_harmony = mkuptr<vec<sptr<Luon>>>();
        for (auto index: signal_indices) {
            luons_in_harmony->push_back(luons[index]);
        }
        return mkuptr<Harmony>(mv(luons_in_harmony));
    }
};

}
