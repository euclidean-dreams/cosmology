#include "cosmology.h"

namespace cosmology {

int OBSERVATION_WIDTH;
int OBSERVATION_HEIGHT;

Cosmology::Cosmology(int observation_width, int observation_height) {
    OBSERVATION_WIDTH = observation_width;
    OBSERVATION_HEIGHT = observation_height;
}

void Cosmology::experience(sp<Signal<float>> &signal) {

}

}
