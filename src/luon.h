#pragma once

#include "axioms.h"

namespace cosmology {

class DeltaChain : public Name {
public:
    lst<float> deltas;

    DeltaChain() {
        for (int i = 0; i < DELTA_CHAIN_LENGTH; i++) {
            deltas.emplace_back(0);
        }
    }

    void record(float energy) {
        auto delta = energy - deltas.front();
        deltas.pop_back();
        deltas.push_front(delta);
    }
};

class Luon : public Name {
public:
    float energy = 0;
    float log_energy = 0;
    int fundamental;
    DeltaChain delta_chain;

    Luon(int fundamental) :
            fundamental{fundamental},
            delta_chain{} {
    }

    void excite(sp<Signal<float>> &signal) {
        energy = signal->get_sample(fundamental);
        log_energy = std::log(energy);
        delta_chain.record(energy);
    }

    float delta() {
        return delta_chain.deltas.front();
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
