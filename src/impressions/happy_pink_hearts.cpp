#include "happy_pink_hearts.h"

namespace cosmology {

HappyPinkHearts::HappyPinkHearts(Psyche &psyche) : psyche{psyche} {

}

up<Lattice> HappyPinkHearts::observe() {
    auto lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
    Coordinate center{lattice->width / 2, lattice->height / 2};
    for(int y = center.y - 33; y < center.y + 33; y++) {
        for(int x = center.x - 33; x < center.x + 33; x++) {
            lattice->set_color(x, y, Color{255, 156, 215});
        }
    }
    return lattice;
}

}
