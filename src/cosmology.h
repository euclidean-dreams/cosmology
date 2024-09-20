#pragma once

#include "axioms.h"
#include "luon.h"
#include "impressions/impression.h"

namespace cosmology {

enum Impressions {
    barometer,
    bubbles,
    calibration,
    happy_pink_hearts,
    hymn,
    juice,
    puff,
    tyr,
    watercolor,
    workshop
};


class Cosmology : public Name {
private:
    uptr<Lattice> current_observation;
    uptr<Psyche> psyche;
    uptr<Impression> impression;

public:
    Cosmology(int observation_width, int observation_height, int luon_count, Impressions impression_choice);

    void experience(sptr<Signal<float>> &signal);

    uptr<Lattice> observe();
};

}
