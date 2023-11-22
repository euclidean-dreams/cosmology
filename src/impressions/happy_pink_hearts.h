#pragma once

#include "axioms.h"
#include "luon.h"

namespace cosmology {

class HappyPinkHearts : public Name {
private:
    Psyche &psyche;

public:
    HappyPinkHearts(Psyche& psyche);

    up<Lattice> observe();
};

}
