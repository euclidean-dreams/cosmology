#pragma once

#include <numbers>
#include "axioms.h"
#include "impression.h"

namespace cosmology {

float PHI = (1 + sqrt(5)) / 2;
#define MAX_GLIMMERS_PER_STEP 100

#ifdef QUETZAL
Color background_color = Color{0, 0, 0};
#else
Color background_color = Color{21, 106, 132};
#endif

class Glimmer : public Name {
private:
    Luon &luon;
    Point origin;
    Color color;
    float direction;

public:
    Glimmer(Luon &luon, Point origin, Color color)
            : luon{luon},
              origin{origin},
              color{color},
              direction{} {
        direction = 333 * luon.index * (2 * M_PI) / scflt(LUON_COUNT);
    }

    void paint(Lattice &lattice) {
        if (luon.smooth_log / 2 > 1) {
            lattice.set_pith(origin.x, origin.y, Pith{color, luon.smooth_log});
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

class Dial : public Name {
public:
    Luon &luon;
    Point origin;
    Point locus;
    Color color;
    lst<Point> path;
    lst<Glimmer> glimmers;
    float t = 0;

    float x_sign = 1;
    float y_sign = 1;

    Dial(Luon &luon, Point origin, Color color)
            : luon{luon},
              origin{origin},
              locus{0, 0},
              color{color},
              path{},
              glimmers{} {
    }

    void paint(Lattice &lattice) {
        for (auto &point: path) {
            lattice.set_pith(point.x, point.y, Pith{color, 2});
        }

        for (auto &glimmer: glimmers) {
            glimmer.paint(lattice);
        }
    }

    void move() {
        while (t < luon.smooth_log * 3) {
            locus.x = x_sign * PHI * t * cos(t);
            locus.y = y_sign * PHI * t * sin(t);
            t += 0.01;

            auto fresh_point = Point{origin.x + locus.x, origin.y + locus.y};
            path.push_front(fresh_point);
        }

        while (path.size() > luon.energy * 4444) {
            path.pop_back();
        }

        // flip spiral when we hit a local min or max
        if ((luon.previous_delta <= 0 && luon.delta > 0) || (luon.previous_delta > 0 && luon.delta <= 0)) {
            t = 0;
            origin = Point{origin.x + locus.x, origin.y + locus.y};
            if (luon.delta > 0) {
                x_sign = 1;
                y_sign = 1;
            } else {
                x_sign = -1;
                y_sign = -1;
            }
        }

        // account for hitting an edge
        if (origin.x < 0 || origin.x > OBSERVATION_WIDTH) {
            origin.x = Randomizer::generate(OBSERVATION_WIDTH);
            origin.y = Randomizer::generate(OBSERVATION_HEIGHT);
        }
        if (origin.y < 0 || origin.y > OBSERVATION_HEIGHT) {
            origin.x = Randomizer::generate(OBSERVATION_WIDTH);
            origin.y = Randomizer::generate(OBSERVATION_HEIGHT);
        }

        // emit glimmers
        for (int i = 0; i < luon.proportion * MAX_GLIMMERS_PER_STEP; i++) {
            glimmers.emplace_back(luon, origin, color);
        }

        while (glimmers.size() > luon.energy * 33) {
            glimmers.pop_back();
        }

        for (auto &glimmer: glimmers) {
            glimmer.move();
        }
    }
};


class Barometer : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Dial>> dials;

public:
    Barometer(Psyche &psyche)
            : psyche{psyche},
              dials{} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        for (auto &luon: *harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto color = HSLColor{luon->index % HSL_HUE_MAX, 77, 77};
            auto dial = mkuptr<Dial>(*luon, Point{x, y}, color.convert_to_rgb());
            dials.push_back(mv(dial));
        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{background_color}, true);
        for (auto &dial: dials) {
            dial->paint(*lattice);
            dial->move();
        }
        return lattice;
    }
};

}
