#pragma once

#include "axioms.h"
#include "luon.h"
#include "impressions/impression.h"

namespace cosmology {

class Cosmology : public Name {
private:
    up<Lattice> current_observation;
    up<Psyche> psyche;
    up<Impression> impression;

public:
    Cosmology(int observation_width, int observation_height, int luon_count);

    void experience(sp<Signal<float>> &signal);

    up<Lattice> observe();
};

}
