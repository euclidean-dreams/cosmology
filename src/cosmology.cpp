#include "cosmology.h"
#include "impressions/puff.h"

namespace cosmology {

int OBSERVATION_WIDTH;
int OBSERVATION_HEIGHT;
int LUON_COUNT;
float MOVEMENT = 1.0;
float MAGNITUDE = 1.0;
float RESONANCE = 1.0;
float TWIST = 1.0;
float CHAOS = 1.0;
uint64_t WORLD_SEED[4] = {333666333, 333999333, 999333999, 999666999};
int THREAD_COUNT = std::thread::hardware_concurrency();
int RENDER_TIMEOUT_MICROSECONDS = 5000;

Cosmology::Cosmology(int observation_width, int observation_height, int luon_count) : current_observation{} {
    OBSERVATION_WIDTH = observation_width;
    OBSERVATION_HEIGHT = observation_height;
    LUON_COUNT = luon_count;
    psyche = mkuptr<Psyche>(LUON_COUNT);
    impression = mkuptr<Puff>(*psyche);
}

void Cosmology::experience(sptr<Signal<float>> &signal) {
    psyche->perceive(signal);
}

uptr<Lattice> Cosmology::observe() {
    return impression->experience();
}

}
