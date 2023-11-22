#include "cosmology.h"

namespace cosmology {

int OBSERVATION_WIDTH;
int OBSERVATION_HEIGHT;
uint64_t world_seed[4] = {333666333, 333999333, 999333999, 999666999};

Cosmology::Cosmology(int observation_width, int observation_height) {
    OBSERVATION_WIDTH = observation_width;
    OBSERVATION_HEIGHT = observation_height;
}

void Cosmology::experience(sp<Signal<float>> &signal) {

}

}
