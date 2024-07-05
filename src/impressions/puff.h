#pragma once

#include "axioms.h"
#include "impression.h"

namespace cosmology {

#define MAX_LOCI 100

class Cloud : public Name {
public:
    Luon &luon;
    Point origin;
    Color color;
    vect<float> thetas;
    float pole;

    Cloud(Luon &luon, Point origin, Color color)
            : luon{luon},
              origin{origin},
              color{color},
              pole{scflt(Randomizer::generate_proportion() * 2 * M_PI)} {
        for (int i = 0; i < MAX_LOCI; i++) {
            thetas.emplace_back(Randomizer::generate_proportion() * 2 * M_PI);
        }
    }

    void paint(Lattice &lattice) {
        for (int i = 0; i < luon.smooth_log * 3 && i < MAX_LOCI; i++) {
            auto locus = Point::from_polar(origin, Randomizer::generate_proportion() * luon.smooth_log * 9, thetas[i]);
            lattice.paint_circle(locus, luon.smooth_log * 3, color);
        }
        for (int i = 0; i < luon.smooth_log * 3 && i < MAX_LOCI; i++) {
            auto locus = Point::from_polar(origin, Randomizer::generate_proportion() * luon.log_energy * 9, thetas[i]);
            locus = Point::from_polar(locus, luon.smooth_log * 3, pole);
            auto mid_color = HSLColor{42, 0, 72}.convert_to_rgb();
            lattice.paint_circle(locus, luon.smooth_log * 3, mid_color);
        }
        for (int i = 0; i < luon.smooth_log * 3 && i < MAX_LOCI; i++) {
            auto locus = Point::from_polar(origin, Randomizer::generate_proportion() * luon.smooth_log * 9, thetas[i]);
            locus = Point::from_polar(locus, luon.smooth_log * 6, pole);
            auto high_color = HSLColor{42, 0, 88}.convert_to_rgb();
            lattice.paint_circle(locus, luon.smooth_log * 3, high_color);
        }
    }

    void move() {
        float distance = scflt(OBSERVATION_WIDTH) / 333 * luon.log_energy * MOVEMENT;
        origin = Point::from_polar(origin, distance, pole);

        if (origin.x < 0 || origin.x > OBSERVATION_WIDTH) {
            origin.x = Randomizer::generate(OBSERVATION_WIDTH);
            origin.y = Randomizer::generate(OBSERVATION_HEIGHT);
        }
        if (origin.y < 0 || origin.y > OBSERVATION_HEIGHT) {
            origin.x = Randomizer::generate(OBSERVATION_WIDTH);
            origin.y = Randomizer::generate(OBSERVATION_HEIGHT);
        }
        pole += TWIST / 999;
    }
};


class Puff : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Cloud>> clouds;

public:
    Puff(Psyche &psyche)
            : psyche{psyche},
              clouds{} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto color = HSLColor{42, 0, 42};
            auto cloud = mkuptr<Cloud>(*luon, Point{x, y}, color.convert_to_rgb());
            clouds.push_back(mv(cloud));
        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Color{0, 0, 0});
        for (auto &cloud: clouds) {
            cloud->paint(*lattice);
            cloud->move();
        }
        return lattice;
    }
};

}
