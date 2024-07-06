#pragma once

#include "axioms.h"
#include "impression.h"

namespace cosmology {

class Calibration : public Impression {
private:
    Psyche &psyche;

public:
    Calibration(Psyche &psyche)
            : psyche{psyche} {
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        lattice->set_color(0, 0, {0, 0, 33});
        lattice->set_color(1, 0, {22, 0, 0});
        lattice->set_color(0, 1, {0, 22, 0});
        lattice->set_color(1, 1, {0, 0, 22});
        lattice->set_color(0, OBSERVATION_HEIGHT - 1, {20, 33, 0});
        lattice->set_color(OBSERVATION_WIDTH - 1, 0, {0, 33, 0});
        lattice->set_color(OBSERVATION_WIDTH - 1, OBSERVATION_HEIGHT - 1, {128, 0, 0});
        return lattice;
    }
};

}
