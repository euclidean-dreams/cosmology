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
    juice,
    puff,
    watercolor,
    workshop,
    here_there_be_dragons,
    obsidian,
    curvature,
    linework,
    gnarl,
    knit,
    meshwork,
    spherics,
    ambiance,
    allegory,

#ifdef threads_found
    hymn,
    tyr,
#endif
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
