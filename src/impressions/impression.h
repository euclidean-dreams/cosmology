#pragma once

#include "axioms.h"
#include "luon.h"

namespace cosmology {

class Impression : public Name {
public:
    virtual uptr<Lattice> experience() = 0;
};

}
