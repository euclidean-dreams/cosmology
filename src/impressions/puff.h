#pragma once

#include "axioms.h"
#include "impression.h"

namespace cosmology {

#define MAX_LOCI 100

class Cloud : public Name {
public:
    Luon &luon;
    Point origin;
    HSLColor color;
    vect<float> thetas;
    float pole;

    Cloud(Luon &luon, Point origin, HSLColor color)
            : luon{luon},
              origin{origin},
              color{color},
              pole{scflt(Randomizer::generate_proportion() * 2 * M_PI)} {
        for (int i = 0; i < MAX_LOCI; i++) {
            thetas.emplace_back(Randomizer::generate_proportion() * 2 * M_PI);
        }
    }

    void paint(Lattice &lattice) {
        auto color_rgb = color.convert_to_rgb();
        for (int i = 0; i < luon.smooth_log * 3 && i < MAX_LOCI; i++) {
            auto locus = Point::from_polar(origin, Randomizer::generate_proportion() * luon.smooth_log * 9, thetas[i]);
            lattice.set_pith(locus.x, locus.y, Pith{color_rgb, luon.smooth_log * 3});
        }
        auto mid_color = color;
        mid_color.saturation += 5;
        mid_color.lightness += 5;
        auto mid_color_rgb = mid_color.convert_to_rgb();
        for (int i = 0; i < luon.smooth_log * 3 && i < MAX_LOCI; i++) {
            auto locus = Point::from_polar(origin, Randomizer::generate_proportion() * luon.log_energy * 9, thetas[i]);
            locus = Point::from_polar(locus, luon.smooth_log * 3, pole);
            lattice.set_pith(locus.x, locus.y, Pith{mid_color_rgb, luon.smooth_log * 3});
        }
        auto high_color = mid_color;
        high_color.saturation += 5;
        high_color.lightness += 5;
        auto high_color_rgb = high_color.convert_to_rgb();
        for (int i = 0; i < luon.smooth_log * 3 && i < MAX_LOCI; i++) {
            auto locus = Point::from_polar(origin, Randomizer::generate_proportion() * luon.smooth_log * 9, thetas[i]);
            locus = Point::from_polar(locus, luon.smooth_log * 6, pole);
            lattice.set_pith(locus.x, locus.y, Pith{high_color_rgb, luon.smooth_log * 3});
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
            auto color = HSLColor{(luon->index + 333) % HSL_HUE_MAX,
                                  50 + Randomizer::generate(50),
                                  Randomizer::generate(50)};
            auto cloud = mkuptr<Cloud>(*luon, Point{x, y}, color);
            clouds.push_back(mv(cloud));
        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{0, 0, 0}}, true);
        for (auto &cloud: clouds) {
            cloud->paint(*lattice);
            cloud->move();
        }
        return lattice;
    }
};

}
