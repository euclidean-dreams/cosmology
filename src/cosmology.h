#pragma once

#include "axioms.h"

namespace cosmology {

class Cosmology : public Name {
public:
    Cosmology(int observation_width, int observation_height);

    void experience(sp<Signal<float>> &signal);
};

}
