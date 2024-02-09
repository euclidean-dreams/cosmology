#include "cosmology.h"
#include "impressions/happy_pink_hearts.h"
#include "impressions/workshop.h"
#include "impressions/bubbles.h"

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

Cosmology::Cosmology(int observation_width, int observation_height, int luon_count) : current_observation{} {
    OBSERVATION_WIDTH = observation_width;
    OBSERVATION_HEIGHT = observation_height;
    LUON_COUNT = luon_count;
    psyche = mkup<Psyche>(LUON_COUNT);
    impression = mkup<Effervescence>(*psyche);
}

void Cosmology::experience(sp<Signal<float>> &signal) {
    psyche->perceive(signal);
}

up<Lattice> Cosmology::observe() {
    return impression->experience();
}

}
