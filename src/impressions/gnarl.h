#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;

namespace gnarl {
class Bough : public Name {
private:
    uptr<Harmony> harmony;
    Point origin;
    HSLColor color;
    float direction;
    vect<Quadrangle> topology;

public:
    Bough(uptr<Harmony> harmony, Point origin, HSLColor color)
        : harmony{mv(harmony)},
          origin{origin},
          color{color},
          direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)},
          topology{} {
    }

    void paint(Lattice &lattice) {
        auto threshold = 0.1;
        for (auto &luon: *harmony->luons) {
            if (luon->energy > threshold) {
                auto root_point = Point{origin.x, origin.y};
                if (!topology.empty()) {
                    root_point = topology.back().l1.p2;
                }
                auto shoot_distance = luon->energy / 99;
                float shoot_theta = M_PI * scflt(luon->index) / scflt(harmony->luons->size());
                auto shoot_point = Point::from_polar(root_point, shoot_distance, shoot_theta);
                auto l1 = Line{root_point, shoot_point};
                auto shoot_squish = luon->energy;
                auto quadrangle = Quadrangle::from_extrusion(l1, shoot_squish);
                topology.push_back(quadrangle);
            }
        }
        for (auto &quadrangle: topology) {
            sketch_quadrangle(quadrangle, lattice);
        };
    }

    static void sketch_quadrangle(Quadrangle quadrangle, Lattice &lattice) {
        sketch_line(quadrangle.l1, lattice);
        sketch_line(quadrangle.l2, lattice);
        sketch_line(quadrangle.l3, lattice);
        sketch_line(quadrangle.l4, lattice);
    }

    static void sketch_line(Line line, Lattice &lattice) {
        int steps = 100;
        for (int i = 0; i <= steps; i++) {
            float ratio = scflt(i) / scflt(steps);
            int x = scint((1 - ratio) * line.p1.x + ratio * line.p2.x);
            int y = scint((1 - ratio) * line.p1.y + ratio * line.p2.y);
            lattice.set_pith(x, y, Pith{{0, 250, 200}, 5});
        }
    }
};

class Gnarl : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Bough>> boughs;

public:
    Gnarl(Psyche &psyche)
        : psyche{psyche} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        auto index = 0;
        float x = Randomizer::generate(OBSERVATION_WIDTH - 1);
        float y = Randomizer::generate(OBSERVATION_HEIGHT - 1);
        auto color = HSLColor{
            Randomizer::generate(HSL_HUE_MAX),
            50 + Randomizer::generate(50),
            33 + Randomizer::generate(50)
        };
        auto bough = mkuptr<Bough>(mv(harmony), Point{x, y}, color);
        boughs.push_back(mv(bough));
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}}, true);
        for (auto &bough: boughs) {
            bough->paint(*lattice);
        }
        return lattice;
    }
};
}
