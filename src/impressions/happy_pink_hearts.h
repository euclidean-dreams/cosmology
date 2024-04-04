#pragma once

#include "axioms.h"
#include "impression.h"

namespace cosmology {

class Hearts {
public:
    Luon &luon;
    Point origin;
    float direction;

    Hearts(Luon &luon, Point origin) : luon{luon}, origin{origin}, direction{} {
        direction = 333 * luon.index * (2 * M_PI) / scflt(LUON_COUNT);
    }

    void paint(Lattice &lattice) {
        auto max_t = luon.energy * TWIST;
        float t = M_PI / 2;
        while (t < max_t) {
            int offset = RESONANCE * std::floor(t / (2 * M_PI));
            auto scale = scflt(OBSERVATION_WIDTH) / 128 * luon.log_energy * MAGNITUDE;
            float distance = (1 - std::sin(t)) * scale;
            float theta = t + offset;
            auto point = Point::from_polar({origin.x, origin.y}, distance, theta);

            uint8_t red = embind(0, 99 * luon.log_energy + luon.delta * 22, 255);
            uint8_t green = embind(0, 33 * luon.log_energy, 255);
            uint8_t blue = embind(0, 66 * luon.log_energy - luon.delta * 11, 255);
            lattice.set_color(point.x, point.y, Color{red, green, blue});

            t += M_PI / (7 * scale) * CHAOS;
        }
    }

    void move() {
        float distance = scflt(OBSERVATION_WIDTH) / 4444 * luon.log_energy * MOVEMENT;
        origin = Point::from_polar(origin, distance, direction);

        if (origin.x < 0 || origin.x > OBSERVATION_WIDTH) {
            origin.x = Randomizer::generate(OBSERVATION_WIDTH);
            origin.y = Randomizer::generate(OBSERVATION_HEIGHT);
        }
        if (origin.y < 0 || origin.y > OBSERVATION_HEIGHT) {
            origin.x = Randomizer::generate(OBSERVATION_WIDTH);
            origin.y = Randomizer::generate(OBSERVATION_HEIGHT);
        }
        direction += TWIST / 999;
    }
};

class HappyPinkHearts : public Impression {
private:
    uptr<Harmony> harmony;
    vec<uptr<Hearts>> hearts;

public:
    HappyPinkHearts(uptr<Harmony> harmony) :
            harmony{mv(harmony)} {
        for (auto &luon: *this->harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto heart = mkuptr<Hearts>(*luon, Point{x, y});
            hearts.push_back(mv(heart));
        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (auto &heart: hearts) {
            heart->move();
            heart->paint(*lattice);
        }
        return lattice;
    }
};

}
