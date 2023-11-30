#pragma once

#include "axioms.h"
#include "luon.h"

namespace cosmology {

class Impression : public Name {
public:
    virtual up<Lattice> experience() = 0;
};

}
