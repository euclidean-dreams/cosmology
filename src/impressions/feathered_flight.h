#pragma once

#include <numbers>
#include "axioms.h"
#include "impression.h"

namespace cosmology {

class FeatheredFlight : public Impression {
private:
    Psyche &psyche;

public:
    FeatheredFlight(Psyche &psyche)
            : psyche{psyche} {
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        Point locus = {scflt(OBSERVATION_WIDTH / 2), scflt(OBSERVATION_HEIGHT / 2)};
        lattice->paint_circle(locus, 3, {0, 33, 77});
        return lattice;
    }
};

}
