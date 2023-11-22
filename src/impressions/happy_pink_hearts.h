#pragma once

#include "axioms.h"
#include "luon.h"

namespace cosmology {


class Heart {
public:
    Luon &luon;
    Point origin;

    Heart(Luon &luon, Point origin) : luon{luon}, origin{origin} {

    }

    void paint(Lattice &lattice) {
        for (int y = origin.y - 3; y < origin.y + 3; y++) {
            for (int x = origin.x - 3; x < origin.x + 3; x++) {
                lattice.set_color(x, y, Color{255, 156, 215});
            }
        }
    }
};

class HappyPinkHearts : public Name {
private:
    up<Harmony> harmony;
    vec<Heart> hearts;

public:
    HappyPinkHearts(up<Harmony> harmony) : harmony{mv(harmony)} {
        for (auto &luon: *this->harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            hearts.emplace_back(*luon, Point{x, y});
        }
    }

    up<Lattice> observe() {
        auto lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (auto &heart: hearts) {
            heart.paint(*lattice);
        }
        return lattice;
    }
};

}
