#include "cosmology.h"

namespace cosmology {

int OBSERVATION_WIDTH;
int OBSERVATION_HEIGHT;
int LUON_COUNT;
uint64_t world_seed[4] = {333666333, 333999333, 999333999, 999666999};

Cosmology::Cosmology(int observation_width, int observation_height, int luon_count) : current_observation{} {
    OBSERVATION_WIDTH = observation_width;
    OBSERVATION_HEIGHT = observation_height;
    LUON_COUNT = luon_count;
    psyche = mkup<Psyche>(LUON_COUNT);
    impression = mkup<HappyPinkHearts>(*psyche);
}

void Cosmology::experience(sp<Signal<float>> &signal) {
    psyche->perceive(signal);
}

up<Lattice> Cosmology::observe() {
    return impression->observe();
}

}
