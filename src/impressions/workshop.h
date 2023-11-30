#pragma once

#include "axioms.h"
#include "impression.h"

namespace cosmology {

class Workshop : public Impression {
private:
    up<Harmony> harmony;

public:
    Workshop(up<Harmony> harmony) :
            harmony{mv(harmony)} {

    }

    up<Lattice> experience() override {
        auto lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        return lattice;
    }
};

}
