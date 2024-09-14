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
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}});
        lattice->set_pith(0, 0, Pith{{0, 0, 33}});
        lattice->set_pith(1, 0, Pith{{22, 0, 0}});
        lattice->set_pith(0, 1, Pith{{0, 22, 0}});
        lattice->set_pith(1, 1, Pith{{0, 0, 22}});
        lattice->set_pith(0, OBSERVATION_HEIGHT - 1, Pith{{20, 33, 0}});
        lattice->set_pith(OBSERVATION_WIDTH - 1, 0, Pith{{0, 33, 0}});
        lattice->set_pith(OBSERVATION_WIDTH - 1, OBSERVATION_HEIGHT - 1, Pith{{128, 0, 0}});
        return lattice;
    }
};

}
