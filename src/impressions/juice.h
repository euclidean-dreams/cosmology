#pragma once

#include "axioms.h"
#include "impression.h"

using namespace cosmology;
namespace juice {

class Lissajous : public Name {
public:
    Luon &luon;
    Luon &upper_partial;
    Point origin;
    lst<Point> loci;
    HSLColor color;
    float direction;

    Lissajous(Luon &luon, Luon &upper_partial, Point origin, HSLColor color)
            : luon{luon},
              upper_partial{upper_partial},
              origin{origin},
              loci{},
              color{color},
              direction{scflt(Randomizer::generate_proportion() * 2 * M_PI)} {

    }

    void paint(Lattice &lattice) {
        if (luon.energy > 0.1) {
            float delta = M_PI / 2;
            float t = 0;
            while (t < 10) {
                float x = MAGNITUDE * 9 * luon.smooth_log * std::sin(luon.energy * t + delta);
                float y = MAGNITUDE * 9 * luon.smooth_log * std::sin(upper_partial.energy * t);
                loci.emplace_front(x, y);
                t += 0.01 / luon.energy;
            }
            while (loci.size() > 999 * luon.energy) {
                loci.pop_back();
            }
            for (auto &point: loci) {
                auto adjusted_point = Point{origin.x + point.x, origin.y + point.y};
                lattice.set_pith(adjusted_point.x, adjusted_point.y, Pith{color.convert_to_rgb(), 1});
            }
        }
    }

    void move() {
        float distance = luon.energy / 9 * MOVEMENT;
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
    }
};

class Juice : public Impression {
private:
    Psyche &psyche;
    vect<uptr<Lissajous>> curves;

public:
    Juice(Psyche &psyche)
            : psyche{psyche},
              curves{} {
        vect<int> luon_indices{};
        luon_indices.reserve(LUON_COUNT);
        for (int i = 0; i < LUON_COUNT; i++) {
            luon_indices.push_back(i);
        }
        auto harmony = this->psyche.create_harmony(luon_indices);
        auto index = 0;
        for (auto &luon: *harmony->luons) {
            float x = Randomizer::generate(OBSERVATION_WIDTH);
            float y = Randomizer::generate(OBSERVATION_HEIGHT);
            auto color = HSLColor{Randomizer::generate(HSL_HUE_MAX),
                                  50 + Randomizer::generate(50),
                                  33 + Randomizer::generate(50)};
            auto upper_partial_index = index * 2;
            if (upper_partial_index < harmony->luons->size()) {
                sptr<Luon> upper_partial = (*harmony->luons)[upper_partial_index];
                auto curve = mkuptr<Lissajous>(*luon, *upper_partial, Point{x, y}, color);
                curves.push_back(mv(curve));
                index++;
            } else {
                break;
            }
        }
    }

    uptr<Lattice> experience() override {
        auto lattice = mkuptr<Lattice>(OBSERVATION_WIDTH, OBSERVATION_HEIGHT, Pith{Color{255, 255, 255}});
        for (auto &curve: curves) {
            curve->move();
            curve->paint(*lattice);
        }
        return lattice;
    }
};

}
