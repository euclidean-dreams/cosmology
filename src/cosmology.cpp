#include "cosmology.h"
#include "impressions/watercolor.h"
#include "impressions/juice.h"
#include "impressions/barometer.h"
#include "impressions/bubbles.h"
#include "impressions/calibration.h"
#include "impressions/happy_pink_hearts.h"
#include "impressions/hymn.h"
#include "impressions/puff.h"
#include "impressions/tyr.h"
#include "impressions/workshop.h"

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

Cosmology::Cosmology(int observation_width, int observation_height, int luon_count, Impressions impression_choice)
        : current_observation{} {
    OBSERVATION_WIDTH = observation_width;
    OBSERVATION_HEIGHT = observation_height;
    LUON_COUNT = luon_count;
    psyche = mkuptr<Psyche>(LUON_COUNT);
    if (impression_choice == Impressions::barometer) {
        impression = mkuptr<barometer::Barometer>(*psyche);
    } else if (impression_choice == Impressions::bubbles) {
        impression = mkuptr<bubbles::Effervescence>(*psyche);
    } else if (impression_choice == Impressions::calibration) {
        impression = mkuptr<calibration::Calibration>(*psyche);
    } else if (impression_choice == Impressions::happy_pink_hearts) {
        impression = mkuptr<happy_pink_hearts::HappyPinkHearts>(*psyche);
    } else if (impression_choice == Impressions::hymn) {
        impression = mkuptr<hymn::Hymn>(*psyche);
    } else if (impression_choice == Impressions::juice) {
        impression = mkuptr<juice::Juice>(*psyche);
    } else if (impression_choice == Impressions::puff) {
        impression = mkuptr<puff::Puff>(*psyche);
    } else if (impression_choice == Impressions::tyr) {
        impression = mkuptr<tyr::Tyr>(*psyche);
    } else if (impression_choice == Impressions::watercolor) {
        impression = mkuptr<watercolor::Watercolor>(*psyche);
    } else if (impression_choice == Impressions::workshop) {
        impression = mkuptr<workshop::Workshop>(*psyche);
    }
}

void Cosmology::experience(sptr<Signal<float>> &signal) {
    psyche->perceive(signal);
}

uptr<Lattice> Cosmology::observe() {
    return impression->experience();
}

}
