#pragma once

#include <numbers>
#include "axioms.h"
#include "impression.h"

namespace cosmology {

class Feather {
public:
    Luon &luon;
    Point origin;

    Feather(Luon &luon, Point origin) : luon{luon}, origin{origin} {
    }

    void paint(Lattice &lattice) {
        uint8_t red = 0;
        uint8_t green = embind(0, luon.smooth_log * 33, 255);
        uint8_t blue = embind(0, luon.smooth_log * 333, 255);
        lattice.set_color(origin.x, origin.y, Color{red, green, blue});
    }

    void move() {

    }
};

class FeatheredFlight : public Impression {
private:
    Psyche &psyche;
    uptr<Harmony> harmony;
    vect<uptr<Feather>> feathers;


public:
    FeatheredFlight(Psyche &psyche)
            : psyche{psyche} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto feather = mkuptr<Feather>(*luon, Point{x, y});
            feathers.push_back(mv(feather));

        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (auto &feather: feathers) {
            feather->move();
            feather->paint(*lattice);
        }
        return lattice;
    }
};

}
