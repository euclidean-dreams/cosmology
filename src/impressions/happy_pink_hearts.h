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
        auto size = std::log(luon.energy);
        for (int y = origin.y - size; y < origin.y + size; y++) {
            for (int x = origin.x - size; x < origin.x + size; x++) {
                lattice.set_color(x, y, Color{255, 156, 215});
            }
        }
    }

    void move() {
        float distance = luon.energy;
        float direction = 33 * luon.fundamental * (2 * M_PI) / scflt(LUON_COUNT);
        origin = Point::from_polar(origin, distance, direction);

        if (origin.x < 0 || origin.x > OBSERVATION_WIDTH) {
            origin.x = scflt(OBSERVATION_WIDTH) / 2;
            origin.y = scflt(OBSERVATION_HEIGHT) / 2;
        }
        if (origin.y < 0 || origin.y > OBSERVATION_HEIGHT) {
            origin.x = scflt(OBSERVATION_WIDTH) / 2;
            origin.y = scflt(OBSERVATION_HEIGHT) / 2;
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
            float x = OBSERVATION_WIDTH / 2;
            float y = OBSERVATION_HEIGHT / 2;
            hearts.emplace_back(*luon, Point{x, y});
        }
    }

    up<Lattice> observe() {
        auto lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (auto &heart: hearts) {
            heart.move();
            heart.paint(*lattice);
        }
        return lattice;
    }
};

}
