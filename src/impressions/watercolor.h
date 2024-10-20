#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;
namespace watercolor {

class Splash : public Name {
public:
    Luon &luon;
    Point origin;
    HSLColor color;
    float direction;
    lst<Point> loci;

    Splash(Luon &luon, Point origin, HSLColor color)
            : luon{luon},
              origin{origin},
              color{color},
              direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)},
              loci{} {

    }

    void paint(Lattice &lattice) {
        while (loci.size() > luon.energy * 3) {
            loci.pop_front();
        }
        while (loci.size() < luon.energy * 3) {
            Point splat{
                    Randomizer::generate(luon.energy * TWIST * OBSERVATION_WIDTH / 33) * Randomizer::generate_sign(),
                    Randomizer::generate(luon.energy * TWIST * OBSERVATION_HEIGHT / 33) * Randomizer::generate_sign()};
            loci.push_front(splat);
        }
        int index = 0;
        for (auto &point: loci) {
            int lightness = color.lightness + index;
            auto current_color = HSLColor{color.hue, color.saturation, lightness};
            auto adjusted_point = Point{origin.x + point.x, origin.y + point.y};
            auto radius = MAGNITUDE * (luon.energy * 9 - index);
            if (radius > MAGNITUDE * OBSERVATION_WIDTH / 9) {
                radius = MAGNITUDE * OBSERVATION_WIDTH / 9;
            }
            float twist = 2 * M_PI * Randomizer::generate_proportion();
            lattice.set_pith(adjusted_point.x, adjusted_point.y,
                             Pith{current_color.convert_to_rgb(), radius, twist});
            point.x += Randomizer::generate(OBSERVATION_WIDTH / 333) * Randomizer::generate_sign();
            point.y += Randomizer::generate(OBSERVATION_WIDTH / 333) * Randomizer::generate_sign();
            index++;
        }
    }

    void move() {
        float distance = std::abs(luon.delta * 3) * MOVEMENT;
        origin = Point::from_polar(origin, distance, direction);
        if (origin.x < 0) {
            origin.x = 0;
            direction += M_PI;
        } else if (origin.x >= OBSERVATION_WIDTH) {
            origin.x = OBSERVATION_WIDTH - 1;
            direction += M_PI;
        }
        if (origin.y < 0) {
            origin.y = 0;
            direction += M_PI;
        } else if (origin.y >= OBSERVATION_HEIGHT) {
            origin.y = OBSERVATION_HEIGHT - 1;
            direction += M_PI;
        }
        auto center_drift_direction = std::atan2(scflt(OBSERVATION_HEIGHT) / 2 - origin.y,
                                                 scflt(OBSERVATION_WIDTH) / 2 - origin.x);
        origin = Point::from_polar(origin, OBSERVATION_WIDTH / 2222, center_drift_direction);
    }
};

class Watercolor : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Splash>> splashes;

public:
    Watercolor(Psyche &psyche)
            : psyche{psyche},
              splashes{} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            float x = OBSERVATION_WIDTH / 2;
            float y = OBSERVATION_HEIGHT / 2;
            auto color = HSLColor{Randomizer::generate(HSL_HUE_MAX),
                                  50 + Randomizer::generate(50),
                                  33 + Randomizer::generate(50)};
            auto splash = mkuptr<Splash>(*luon, Point{x, y}, color);
            splashes.push_back(mv(splash));
        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}}, true);
        for (auto &splash: splashes) {
            splash->move();
            splash->paint(*lattice);
        }
        return lattice;
    }
};

}
