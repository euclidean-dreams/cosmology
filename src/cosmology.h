#pragma once

#include "axioms.h"
#include "luon.h"
#include "impressions/impression.h"

namespace cosmology {

class Cosmology : public Name {
private:
    uptr<Lattice> current_observation;
    uptr<Psyche> psyche;
    uptr<Impression> impression;

public:
    Cosmology(int observation_width, int observation_height, int luon_count);

    void experience(sptr<Signal<float>> &signal);

    uptr<Lattice> observe();
};

}
