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
        auto size = scflt(OBSERVATION_WIDTH) / 333 * luon.log_energy * COSMOLOGY_EXCITATION;
        for (int y = origin.y - size; y < origin.y + size; y++) {
            for (int x = origin.x - size; x < origin.x + size; x++) {
                auto fade = std::abs(x - origin.x) + std::abs(y - origin.y);
                uint8_t red = embind(0, 99 * luon.log_energy - fade, 255);
                uint8_t green = embind(0, 33 * luon.log_energy - fade, 255);
                uint8_t blue = embind(0, 66 * luon.log_energy - fade, 255);
                lattice.set_color(x, y, Color{red, green, blue});
            }
        }
    }

    void move() {
        float distance = scflt(OBSERVATION_WIDTH) / 4444 * luon.log_energy * COSMOLOGY_EXCITATION;
        float direction = 333 * luon.fundamental * (2 * M_PI) / scflt(LUON_COUNT);
        origin = Point::from_polar(origin, distance, direction);

        // gravity
        origin = Point::from_polar(origin, luon.fundamental / 36963, -direction);

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
