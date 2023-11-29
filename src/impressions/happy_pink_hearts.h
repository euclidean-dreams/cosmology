#pragma once

#include "axioms.h"
#include "luon.h"

namespace cosmology {

class Glimmer : public Name {
public:
    Luon &luon;
    Point origin;

    Glimmer(Luon &luon, Point origin) : luon{luon}, origin{origin} {

    }

    void move() {
        float distance = scflt(OBSERVATION_WIDTH) / 4444 * luon.log_energy * DIRECTION;
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


class Heart {
public:
    Glimmer &glimmer;

    Heart(Glimmer &glimmer) : glimmer{glimmer} {

    }

    void paint(Lattice &lattice) {
        auto size = scflt(OBSERVATION_WIDTH) / 333 * glimmer.luon.log_energy * MAGNITUDE;
        for (int y = glimmer.origin.y - size; y < glimmer.origin.y + size; y++) {
            for (int x = glimmer.origin.x - size; x < glimmer.origin.x + size; x++) {
                auto fade = std::abs(x - glimmer.origin.x) + std::abs(y - glimmer.origin.y);
                uint8_t red = embind(0, 99 * glimmer.luon.log_energy - fade, 255);
                uint8_t green = embind(0, 33 * glimmer.luon.log_energy - fade, 255);
                uint8_t blue = embind(0, 66 * glimmer.luon.log_energy - fade, 255);
                lattice.set_color(x, y, Color{red, green, blue});
            }
        }
    }

};

class HappyPinkHearts : public Name {
private:
    up<Harmony> harmony;
    vec<up<Glimmer>> glimmering;
    vec<Heart> hearts;

public:
    HappyPinkHearts(up<Harmony> harmony) : harmony{mv(harmony)} {
        for (auto &luon: *this->harmony->luons) {
            float x = OBSERVATION_WIDTH / 2;
            float y = OBSERVATION_HEIGHT / 2;
            auto glimmer = mkup<Glimmer>(*luon, Point{x, y});
            glimmering.push_back(mv(glimmer));
        }
        for (auto &glimmer: glimmering) {
            hearts.emplace_back(*glimmer);
        }
    }

    up<Lattice> observe() {
        auto lattice = mkup<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (auto &glimmer: glimmering) {
            glimmer->move();
        }
        for (auto &heart: hearts) {
            heart.paint(*lattice);
        }
        return lattice;
    }
};

}
