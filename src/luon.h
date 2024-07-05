#pragma once

#include "axioms.h"

namespace cosmology {

#define SMOOTH_LOG_HISTORY_LENGTH 3

class Luon : public Name {
public:
    float energy = 0;
    float log_energy = 0;
    int index;
    float delta = 0;
    float previous_delta = 0;
    float log_delta = 0;
    float previous_energy = 0;
    float smooth_log = 0;
    lst<float> prior_log_energies;
    float proportion = 0;

    Luon(int index) :
            index{index},
            prior_log_energies{} {
    }

    void excite(sptr<Signal<float>> &signal, float signal_energy) {
        previous_energy = energy;
        while (prior_log_energies.size() > SMOOTH_LOG_HISTORY_LENGTH) {
            prior_log_energies.pop_back();
        }
        prior_log_energies.push_front(log_energy);
        energy = signal->get_sample(index);
        log_energy = std::log(energy + 1);
        previous_delta = delta;
        delta = energy - previous_energy;
        if (delta > 0) {
            log_delta = std::log(delta + 1);
        } else {
            log_delta = -std::log(1 - delta);
        }
        float smooth_log_sum = 0;
        for (auto prior_energy: prior_log_energies) {
            smooth_log_sum += prior_energy;
        }
        smooth_log = smooth_log_sum / prior_log_energies.size() * std::log(index + 1);
        proportion = energy / signal_energy;
    }
};


class Harmony : public Name {
public:
    uptr<vect<sptr<Luon>>> luons;

    Harmony(uptr<vect<sptr<Luon>>> luons) : luons{mv(luons)} {

    }
};


class Psyche : public Name {
private:
    vect<sptr<Luon>> luons;

public:
    Psyche(int luon_count) : luons{} {
        for (int fundamental = 0; fundamental < luon_count; fundamental++) {
            luons.push_back(mksptr<Luon>(fundamental));
        }
    }

    void perceive(sptr<Signal<float>> &signal) {
        float signal_energy = 0;
        for (auto sample: *signal) {
            signal_energy += sample;
        }
        for (auto &luon: luons) {
            luon->excite(signal, signal_energy);
        }
    }

    uptr<Harmony> create_harmony(vect<int> &signal_indices) {
        auto luons_in_harmony = mkuptr<vect<sptr<Luon>>>();
        for (auto index: signal_indices) {
            luons_in_harmony->push_back(luons[index]);
        }
        return mkuptr<Harmony>(mv(luons_in_harmony));
    }
};

}
